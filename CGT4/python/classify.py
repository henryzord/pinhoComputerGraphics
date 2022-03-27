import argparse
import sys

import numpy as np
import pandas as pd
from sklearn.metrics import accuracy_score
from sklearn.ensemble import RandomForestClassifier


def main(args):
    treino = pd.read_csv(args.training_set)
    teste = pd.read_csv(args.test_set)
    atributos = np.asarray(treino.columns)

    if not np.array_equal(atributos, teste.columns):
        print('Os arquivos possuem atributos distintos')
        sys.exit(1)

    id = atributos[0]  # O primeiro argumento é o identificador da máscara
    preditivos = atributos[1: - 1]  # atributos do meio são preditivos
    alvo = atributos[-1]  # último atributo é classe

    print('Atributo identificador:', id)
    print('Atributos preditivos:', ', '.join(preditivos))
    print('Atributo alvo:', alvo)
    print('Numero de registros de subtreino:', len(treino))
    print('Numero de registros de subteste:', len(teste))

    masks = treino[id].unique()
    for mask in masks:
        subtreino = treino.loc[treino[id] == mask]
        subteste = teste.loc[teste[id] == mask]

        # treina o classificador
        clf = RandomForestClassifier(n_estimators=100)
        clf.fit(subtreino[preditivos], subtreino[alvo])
    
        # teste
        respostas = clf.predict(subteste[preditivos])

        acc = accuracy_score(subteste[alvo], respostas, normalize=True) * 100
        print('\tAcurácia para máscara %s: %0.2f%%' % (mask, acc))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='Script for classifying patches based on descriptors.'
    )

    parser.add_argument(
        '--training-set', action='store', required=True,
        help='Path to training set .csv'
    )

    parser.add_argument(
        '--test-set', action='store', required=True,
        help='Path to test set .csv'
    )

    some_args = parser.parse_args()
    main(some_args)
