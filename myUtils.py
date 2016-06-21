# -*- coding: utf-8 -*-

# Função transpose
# Argumentos: Uma matriz (array de arrays)
# Retorna: A matriz transposta (array of arrays) de A 

def transpose(A):
    #TODO
    return A;
    

# Função multMatrix
# Argumentos: Matrizes (array de arrays) A e B
# Returna: O produto AB 

def multMatrix(A, B):
    #TODO
    return A;


# Função indentity
# Argumentos: Inteiro n 
# Returna: Uma matriz identidade nxn

def identity(n):
    I = []
    for i in range(n):
        I.append([1 if j == i else 0 for j in range(n)])
    return I;
    

# Função readImage
# Argumentos: fileName - Nome do arquivo
# Returna: A matriz lida do arquivo (ou retorna None caso não exista a
#          imagem)
# TODO: Matt, você manja essa parte 

def readImage(fileName):
    
    #Place holder
    A = identity(4)
    return A
    
