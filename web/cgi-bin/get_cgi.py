#!/usr/bin/env python3

def grandezza_contenuto_binario(file_path):
    try:
        ret = 0
        with open(file_path, 'rb') as file:
            # Leggi il contenuto binario del file
            contenuto_binario = file.read()

            # Stampa il contenuto binario sul terminale
            ret += len(contenuto_binario)
        return ret

    except FileNotFoundError:
        print(f"Errore: Il file '{file_path}' non è stato trovato.")
    except Exception as e:
        print(f"Errore durante l'apertura del file: {e}")

def stampa_contenuto_binario(file_path):
    try:
        with open(file_path, 'rb') as file:
            # Leggi il contenuto binario del file
            contenuto_binario = file.read()

            # Stampa il contenuto binario sul terminale
            print(contenuto_binario)

    except FileNotFoundError:
        print(f"Errore: Il file '{file_path}' non è stato trovato.")
    except Exception as e:
        print(f"Errore durante l'apertura del file: {e}")

file_path = './web/img/Gatto.jpeg'
header = ("Content-Type:\r\n")
header += "Content-Length: " + str(grandezza_contenuto_binario(file_path)) + "\r\n\r\n"

print(header)

# Sostituisci 'percorso_del_tuo_file.bin' con il percorso del tuo file binario
stampa_contenuto_binario(file_path)
