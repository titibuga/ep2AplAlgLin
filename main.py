#!/usr/bin/env python
# -*- coding: utf-8 -*-
from myUtils import *
from hessenberg import HessenMatrix

def main():
    #For now just fuck memory 
    A = readImage("a")
    At = transpose(A)
    AtA = multMatrix(At, A)
    AAt = multMatrix(At, A)
    
    H = HessenMatrix(A)


# Função qrMethod
# Argumentos: Um objeto HessenMatrix
# Retorna: Matrizes Q' e R' depois de alguma iterações do método QR

# TODO: Talvez receber também paramêtro de erro para saber quando
#       parar? 

def qrMethod(H):
    Q2 = [1]
    R2 = [1] 


    return (Q2, R2)

# Função qrDecomp
# Argumentos: Um objeto HessenMatrix
# Retorna: Matrizes Q e R da decomposição QR usando householder

def qrDecomp(H):
    Q2 = [1]
    R2 = [1] 


    return (Q2, R2)
    
# Função do python para executar a função main() quando esse script
# for chamado direto do terminal
if __name__ == "__main__":
    main()
