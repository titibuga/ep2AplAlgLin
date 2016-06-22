#!/usr/bin/env python
# -*- coding: utf-8 -*-
from myUtils import *
from hessenberg import HessenMatrix

def main():
    #For now just fuck memory 
    A = readImage("images/lena.png")
    print(A)
    
    #At = transpose(A)
    #AtA = multMatrix(At, A)
    #AAt = multMatrix(At, A)
    a = [[1,1,3],
         [2,3,7],
         [8,4,5]]
    b = [[2,0,0],[0,2,0],[0,0,2]]

    c = multMatrix(a,b)
    print(c)
    c2 = transpose(c)
    print(c2)
    
    saveImage("images/batata.png",255-A)
    
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
