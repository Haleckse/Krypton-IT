import threading
import queue
import random


def is_prime(n, k=5):  # Test de primalité utilisant le test de Miller-Rabin
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0:
        return False

    # Trouver d tel que n-1 = d * 2^r
    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2

    # Test de Miller-Rabin k fois
    for _ in range(k):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)  # a^d % n
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def generate_random_prime(bits=16):
    while True:
        # Générer un nombre impair aléatoire de la taille spécifiée
        candidate = random.getrandbits(bits) | 1
        if is_prime(candidate):
            return candidate


# Calcul de la puissance modulaire
def puissance_mod_n(a: int, e: int, n: int) -> int:
    result = 1
    base = a % n
    while e > 0:
        if e % 2 == 1:
            result = (result * base) % n
        e = e // 2
        base = (base * base) % n
    return result

# Fonction d’Alice
def alice(p, g, queue_bob, queue_alice):
    a = generate_random_prime(bits=16)  # Privé à Alice
    print("Alice choisit le nombre", a, " (connu uniquement par Alice)")
    A = puissance_mod_n(g, a, p)
    print("Alice envoie A =", A, "à Bob (connu de l'espion Eve)")
    queue_bob.put(A)  # Alice envoie A

    B = queue_alice.get()  # Alice reçoit B de Bob
    print("Alice reçoit B =", B, "de Bob")
    
    shared_key_alice = puissance_mod_n(B, a, p)
    print("Clé partagée calculée par Alice =", shared_key_alice)

# Fonction de Bob
def bob(p, g, queue_bob, queue_alice):
    b = generate_random_prime(bits=16)  # Privé à Bob
    print("Bob choisit le nombre", b, " (connu uniquement par Bob)")
    B = puissance_mod_n(g, b, p)
    print("Bob envoie B =", B, "à Alice (connu de l'espion Eve)")
    queue_alice.put(B)  # Bob envoie B
    
    A = queue_bob.get()  # Bob reçoit A de Alice
    print("Bob reçoit A =", A, "de Alice")
    
    shared_key_bob = puissance_mod_n(A, b, p)
    print("Clé partagée calculée par Bob =", shared_key_bob)

def main():
    # Lecture des paramètres depuis le fichier généré
    with open("Output/param.txt", "r") as file:
        params = file.readlines()
    p = int(params[0].split("=")[1].strip())
    g = int(params[1].split("=")[1].strip())
    
    print(f"Paramètres connus d'Alice, Bob et Eve : p = {p}, g = {g}\n")

    # Création des queues pour simuler la communication entre Alice et Bob
    queue_bob = queue.Queue()
    queue_alice = queue.Queue()
    
    # Création des threads pour Alice et Bob
    thread_alice = threading.Thread(target=alice, args=(p, g, queue_bob, queue_alice))
    thread_bob = threading.Thread(target=bob, args=(p, g, queue_bob, queue_alice))
    
    # Lancement des threads
    thread_alice.start()
    thread_bob.start()
    
    # Attente de la fin des threads
    thread_alice.join()
    thread_bob.join()

if __name__ == "__main__":
    main()
