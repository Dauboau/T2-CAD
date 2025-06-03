import random

# Configurações
num_linhas = 1000000 # número de linhas a serem geradas (altere se necessário)

# Gera a lista de caracteres permitidos: ASCII de 32 até 127 (inclusive)
caracteres_permitidos = ''.join(chr(i) for i in range(32, 128))

with open("entrada.txt", "w", encoding="utf-8") as f:
    for _ in range(num_linhas):
        numero = random.randint(1, 10**9)  # Gera um inteiro positivo aleatório
        f.write(f"{numero}\n")