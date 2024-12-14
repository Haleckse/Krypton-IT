import threading
import queue
import random
import argparse
import os

print("Chemin courant :", os.getcwd())
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

def generate_random_prime(bits):
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

def remove_bit(n):
    return n >> 1  # Décalage à droite de 1 bit
def add_bit(n): 
    return (n << 1) | 1  # Equivalent de n*2 + 1
def number_of_bits(n):
    return n.bit_length()

def save_key_to_file(shared_key):
    """Écrit la clé partagée dans un fichier."""
    output_dir = "./Output"
    output_file = os.path.join(output_dir, "key.txt")
    
    # Assurer que le dossier Output existe
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # Écriture de la clé dans le fichier
    with open(output_file, "w") as file:
        file.write(f"{shared_key}\n")
    print("\033[38;5;47mClé partagée écrite dans ../Output/key.txt\033[0m")

# Fonction d’Alice
def alice(p, g, queue_bob, queue_alice, longueur_clef):
    a = generate_random_prime(bits=longueur_clef)  # Privé à Alice
    print("\033[38;5;205mAlice choisit le nombre", a, "(connu uniquement par Alice)\033[0m")
    A = puissance_mod_n(g, a, p)
    print("\033[38;5;205mAlice envoie A =", A, "à Bob \033[38;5;55m(connu de l'espion Eve)\033[0m")
    queue_bob.put(A)  # Alice envoie A

    B = queue_alice.get()  # Alice reçoit B de Bob
    print("\033[38;5;205mAlice reçoit B =", B, "de Bob\033[0m")
    
    shared_key_alice = puissance_mod_n(B, a, p)
    while number_of_bits(shared_key_alice) != longueur_clef : 
        if number_of_bits(shared_key_alice) < longueur_clef : 
            shared_key_alice = add_bit(shared_key_alice)
        else : shared_key_alice = remove_bit(shared_key_alice)
    print("\033[38;5;205mClé partagée calculée par Alice =", shared_key_alice, "\033[0m")

# Fonction de Bob
def bob(p, g, queue_bob, queue_alice, longueur_clef):
    b = generate_random_prime(bits=longueur_clef)  # Privé à Bob
    print("\033[34mBob choisit le nombre", b, "(connu uniquement par Bob)\033[0m")
    B = puissance_mod_n(g, b, p)
    print("\033[34mBob envoie B =", B, " \033[38;5;55m(connu de l'espion Eve)\033[0m")
    queue_alice.put(B)  # Bob envoie B
    
    A = queue_bob.get()  # Bob reçoit A de Alice
    print("\033[34mBob reçoit A =", A, "de Alice\033[0m")
    
    shared_key_bob = puissance_mod_n(A, b, p)
    while number_of_bits(shared_key_bob) != longueur_clef : 
        if number_of_bits(shared_key_bob) < longueur_clef : 
            shared_key_bob = add_bit(shared_key_bob)
        else : shared_key_bob = remove_bit(shared_key_bob)
    print("\033[34mClé partagée calculée par Bob =", shared_key_bob, "\033[0m")
    save_key_to_file(shared_key_bob)

def main():

    parser = argparse.ArgumentParser(description="Protocole Diffie-Hellman avec longueur de clé paramétrable.")
    parser.add_argument("-l", "--longueur_clef", type=int, default=24, help="Longueur de la clé en bits (par défaut 24 bits)")
    
     # Lecture des arguments
    args = parser.parse_args()
    # Récupérer la longueur de clé de l'utilisateur
    longueur_clef = args.longueur_clef
    print(f"Longueur de la clé choisie: {longueur_clef} bits")

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
    thread_alice = threading.Thread(target=alice, args=(p, g, queue_bob, queue_alice, longueur_clef))
    thread_bob = threading.Thread(target=bob, args=(p, g, queue_bob, queue_alice, longueur_clef))
    
    # Lancement des threads
    thread_alice.start()
    thread_bob.start()
    
    # Attente de la fin des threads
    thread_alice.join()
    thread_bob.join()

if __name__ == "__main__":
    main()
