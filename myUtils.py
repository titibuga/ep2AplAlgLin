# -*- coding: utf-8 -*-
import numpy as np
import struct
import os

# Função transpose
# Argumentos: Uma matriz (array de arrays)
# Retorna: A matriz transposta (array of arrays) de A 

def transpose(A):
    T  = [[A[j][i] for j in range(len(A))] for i in range(len(A[0]))]
    return T

# Função multMatrix
# Argumentos: Matrizes (array de arrays) A e B
# Returna: O produto AB 

def multMatrix(A, B):
    C = [[ 0 for j in range(len(B[0]))] for i in range(len(A))]
    
    for i in range(len(A)):
        for j in range(len(B[0])):
            for k in range(len(B)):
                C[i][j] += A[i][k] * B[k][j]
                
    return C


# Função indentity
# Argumentos: Inteiro n 
# Returna: Uma matriz identidade nxn

def identity(n):
    I = []
    for i in range(n):
        I.append([1 if j == i else 0 for j in range(n)])
    return I
    

# Função readImage
# Argumentos: fileName - Nome do arquivo
# Returna: A matriz lida do arquivo (ou retorna None caso não exista a
#          imagem)


def readImage(fileName):
    import PIL.Image
    return pil2array(PIL.Image.open(fileName).convert('L'))

def saveImage(filename, ima):
    import scipy.misc as mi
    mi.imsave(filename, ima)

def pil2array(pil):
    w, h = pil.size
    shape = (h, w)
    image_dtype = np.uint8
    if pil.mode == '1':
        image_dtype = np.bool
    elif pil.mode == 'L':
        image_dtype = np.uint8
    elif pil.mode == 'I;16B':
        image_dtype = np.uint16
    elif pil.mode  in ('F', 'I'):
        image_dtype = np.uint32
    elif pil.mode == 'P':
        pil = pil.convert('RGB')
        shape = (h,w,3)
    elif pil.mode in ('RGB', 'YCbCr'):
        shape = (h,w,3)
        image_dtype = np.uint8
    elif pil.mode in ('RGBA', 'CMYK'):
        shape = (h,w,4)
        image_dtype = np.uint8
    else:
        raise TypeError("Invalid or unimplemented PIL image mode '%s'" % pil.mode)

    return np.array(pil.getdata(), image_dtype).reshape(shape)
