import sys
import operator
if __name__ == "__main__":
    idx_linea = 0;
    lineas = list(sys.stdin)
    num_lineas = len(lineas)
    num_numeros = int(lineas[idx_linea])
#    print("el num de nums %u" % num_numeros)
    idx_linea += 1
    numeros = [int(x) for x in lineas[idx_linea].strip().split(" ")]
    idx_linea += 1
    num_consuls = int(lineas[idx_linea])
#    print("el num de consuls %u" % num_consuls)
    idx_linea += 1
    for num_caca in range(num_consuls):
        (idx_ini, idx_fin) = [int(x) for x in lineas[idx_linea].strip().split(" ")]
#        print("la consulta %u-%u" % (idx_ini , idx_fin+1))
        mierda = set(numeros[idx_ini :idx_fin +1])
        print(min(mierda))
        idx_linea += 1
