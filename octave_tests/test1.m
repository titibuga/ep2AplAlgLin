A = [ [0 255 0]' [255 255 255]' [0 255 0]' ]

aat = A*A'

[P, T] = hess(aat)
[Q, R] = eigs(T)

ata = A'*A

[P2, T2] = hess(ata)
[Q2, R2] = eigs(T)

U = P2*Q2
V = P*Q

R(3,3) = 0
sig = sqrt(R)

ANSW = U*sigV

#r = sqrt(2)/2
#G1 = [[r -r 0]' [r r 0]' [0 0 1]']
#G2 = [[1 0 0]' [0 0 -1]' [0 1 0]']
