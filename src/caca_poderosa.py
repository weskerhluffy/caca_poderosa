import sys
import operator
from math import floor

def quita_mierda(cardinalidades, numero):
    cardinalidad_act = cardinalidades[numero]
    viejo_valor = cardinalidad_act * cardinalidad_act * numero
    cardinalidad_nueva = cardinalidad_act - 1
    assert(cardinalidad_act >= 0)
    nuevo_valor = cardinalidad_nueva * cardinalidad_nueva * numero
    cardinalidades[numero] = cardinalidad_nueva
    return nuevo_valor - viejo_valor

def pon_mierda(cardinalidades, numero):
    cardinalidad_act = cardinalidades.setdefault(numero, 0)
    viejo_valor = cardinalidad_act * cardinalidad_act * numero
    cardinalidad_nueva = cardinalidad_act + 1
    nuevo_valor = cardinalidad_nueva * cardinalidad_nueva * numero
    cardinalidades[numero] = cardinalidad_nueva
    return nuevo_valor - viejo_valor

if __name__ == "__main__":
    idx_linea = 0;
    lineas = list(sys.stdin)
    num_lineas = len(lineas)
    (num_numeros, num_consuls) = [int(x) for x in lineas[idx_linea].strip().split(" ")]
#    print("el num de nums %u" % num_numeros)
#    print("el num de consuls %u" % num_consuls)
    idx_linea += 1
    numeros = [int(x) for x in lineas[idx_linea].strip().split(" ")]
    numeros = [0] + numeros
    idx_linea += 1
#    print("este mundo %s"%(numeros))
    consuls = []
    tam_bloque = floor(num_numeros ** (0.5))
    print("el tam de bloq %u"%tam_bloque)
    for idx_mierda, num_caca in enumerate(range(num_consuls)):
        (idx_ini, idx_fin) = [int(x) for x in lineas[idx_linea].strip().split(" ")]
#        print("la consulta %u-%u" % (idx_ini , idx_fin+1))
#        mierda = set(numeros[idx_ini :idx_fin +1])
#        print(min(mierda))
        consuls.append([idx_ini, idx_fin, tam_bloque, idx_mierda, idx_mierda])
        idx_linea += 1
#    print("x amor i %s"%(consuls))
    consuls_ord = sorted(consuls, key=lambda x: ((x[0] // x[2]), x[1]))
#    print("i glor %s"%(consuls_ord))
    for idx_mierda, consul in enumerate(consuls_ord):
        consul[-1] = idx_mierda
#    print("vivi %s"%(consuls_ord))
    consuls = sorted(consuls_ord, key=lambda x: ((x[0] // x[2]), x[1], x[0], x[-1]))
#    print(":v %s"%(consuls))
    cardinalidades = {}
    suma_act = 0
    idx_ini_act = idx_fin_act = consuls[0][0]
    cardinalidades[numeros[idx_ini_act]] = 1
    suma_act = numeros[idx_ini_act]
    mierdas = []
    for consul in consuls:
        (idx_ini, idx_fin, _, idx_chida, _) = consul
        print("mierda %u-%u" % (idx_ini, idx_fin))
        while(idx_ini_act > idx_ini):
            idx_ini_act -= 1
            suma_act += pon_mierda(cardinalidades, numeros[idx_ini_act])

        while(idx_fin_act < idx_fin):
            idx_fin_act += 1
            suma_act += pon_mierda(cardinalidades, numeros[idx_fin_act])

        while(idx_ini_act < idx_ini):
            suma_act += quita_mierda(cardinalidades, numeros[idx_ini_act])
            idx_ini_act += 1

        while(idx_fin_act > idx_fin):
            suma_act += quita_mierda(cardinalidades, numeros[idx_fin_act])
            idx_fin_act -= 1
#        print("%lu"%suma_act)
        mierdas.append((idx_chida, suma_act))
    mierdas = sorted(mierdas, key=lambda x: x[0])
    for mierda in mierdas:
        print("%ld" % mierda[1])
