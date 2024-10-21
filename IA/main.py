# Importa as bibliotecas utilizadas pelo código do projeto
# Requests para realizar a comunicação entre o código e a IA
import requests

# Time para controlar o tempo de execução do código
import time

# Json para lidar com conersões Object->Json e vice versa
import json

# Pandas para importar a lista do treinamento da IA do projeto
import pandas as pd

# Bibliotecas do Sklearn
# Train_Test_Split para dividir os dados de treinamento entre treino, validação e teste 
from sklearn.model_selection import train_test_split
# Cinco modelos de IA utilizados no código 
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.neural_network import MLPClassifier
from sklearn.ensemble import ExtraTreesClassifier
# Accuracy_Score para calcular a preçisão dos modelos de IA
from sklearn.metrics import accuracy_score
# GridSearchCV para descobrir os melhores hiperparametros dos cinco modelos de IA
from sklearn.model_selection import GridSearchCV



# Carrega os dados de treinamento, validação e teste analisados pelos sensores
InfoTreinoTeste = pd.read_csv("./database.csv")

# Seleciona as colunas de entrada e saída da IA
x = InfoTreinoTeste.drop(["Indefinido", "Metal", "Vidro", "Plastico", "Papel", "D1", "D2"], axis=1)
y = InfoTreinoTeste.drop(["Frequencia1", "Frequencia2", "Frequencia3", "Frequencia4", "D1", "D2"], axis=1)

# Exibe as informações de entrada
print(x)
# Exibe as informações de saida
print(y)

# Divide os dados em treinamento, validação e teste (70%,15%,15%)
# Divide a lista entre treinamento (70%) e, temporariamente, o restante da lista (30%)
x_treino, x_temporario, y_treino, y_temporario = train_test_split(x, y, test_size=0.3, random_state=1)
# Divide os 30% temporarios da lista entre validacao (15%) e teste (15%)
x_validacao, x_teste, y_validacao, y_teste = train_test_split(x_temporario, y_temporario, test_size=0.5, random_state=1)



# Defina a grade de hiperparâmetros que devem ser analisados para cada modelo
ParametrosGridModeloArvore = {
    # Numero de arvores no treinamento
    'n_estimators': [10, 50, 100, 200, 300, 500],
    # Limite de profundidade do modelo (Ilimitado, ...)
    'max_depth': [None, 5, 10, 20, 30, 40, 50],
}

ParametrosGridModeloVizinhoMaisProximo = {
    # Numero de vizinhos que afetam na decisão
    'n_neighbors': [2, 3, 5, 7, 9],
    # Tipo de peso a ser considerado (Igualitario ou com base em distâncias)
    'weights': ['uniform', 'distance', 'custom'],
}

ParametrosGridModeloRedeNeural = {
    # Numero de neurônicos em cada camada oculta
    'hidden_layer_sizes': [(10,), (20,), (30,), (40,), (50,)],
    # Funcao de ativação dos neurônios
    'activation': ['relu', 'tanh', 'sigmoid'],
    # Numero de gerações do algoritmo
    'max_iter': [50, 100, 200, 300, 400, 500],
}

ParametrosGridModeloMLP = {
    # Numero de neurônicos em cada camada oculta
    'hidden_layer_sizes': [(10,), (20,), (30,), (40,), (50,)],
    # Funcao de ativação dos neurônios
    'activation': ['relu', 'tanh', 'sigmoid'],
    # Numero de gerações do algoritmo
    'max_iter': [50, 100, 200, 300, 400, 500],
}

ParametrosGridModeloExtraTrees = {
    # Numero de árvores no treinamento
    'n_estimators': [50, 100, 200, 300, 500, 750],
    # Limite de profundidade do modelo (Ilimitado, ...)
    'max_depth': [None, 5, 10, 20, 30, 40, 50],
}

# Cria objetos GridSearchCV para otimizar os hiperparâmetros de cada modelo
GridSearchArvore = GridSearchCV(RandomForestClassifier(), ParametrosGridModeloArvore, cv=5)
GridSearchVizinho = GridSearchCV(KNeighborsClassifier(), ParametrosGridModeloVizinhoMaisProximo, cv=5)
GridSearchRede_neural = GridSearchCV(MLPClassifier(), ParametrosGridModeloRedeNeural, cv=5)
GridSearchMlp = GridSearchCV(MLPClassifier(), ParametrosGridModeloMLP, cv=5)
GridSearchExtraTrees = GridSearchCV(ExtraTreesClassifier(), ParametrosGridModeloExtraTrees, cv=5)

# Realiza a busca em grade para encontrar os melhores hiperparâmetros de cada modelo
GridSearchArvore.fit(x_treino, y_treino)
GridSearchVizinho.fit(x_treino, y_treino)
GridSearchRede_neural.fit(x_treino, y_treino)
GridSearchMlp.fit(x_treino, y_treino)
GridSearchExtraTrees.fit(x_treino, y_treino)

# Obtem os melhores modelos com base nos melhores hiperparâmetros
MelhorModeloArvore = GridSearchArvore.best_estimator_
MelhorModeloVizinhoMaisProximo = GridSearchVizinho.best_estimator_
MelhorModeloRedeNeural = GridSearchRede_neural.best_estimator_
MelhorModeloMLP = GridSearchMlp.best_estimator_
MelhorModeloExtraTrees = GridSearchExtraTrees.best_estimator_



# Treina os modelos selecionados
MelhorModeloArvore.fit(x_treino, y_treino)
MelhorModeloVizinhoMaisProximo.fit(x_treino, y_treino)
MelhorModeloRedeNeural.fit(x_treino, y_treino)
MelhorModeloMLP.fit(x_treino, y_treino)
MelhorModeloExtraTrees.fit(x_treino, y_treino)

# Avalia os melhores modelos com base no conjunto de teste
PrevisaoTesteModeloArvore = MelhorModeloArvore.predict(x_teste)
PrevisaoTesteVizinhoMaisProximo = MelhorModeloVizinhoMaisProximo.predict(x_teste)
PrevisaoTesteRedeNeural = MelhorModeloRedeNeural.predict(x_teste)
PrevisaoTesteMLP = MelhorModeloMLP.predict(x_teste)
PrevisaoTesteExtraTrees = MelhorModeloExtraTrees.predict(x_teste)

# Avaliação da precisão no conjunto de teste
PrecisaoModeloArvore = accuracy_score(y_teste, PrevisaoTesteModeloArvore)
PrecisaoModeloVizinhoMaisProximo = accuracy_score(y_teste, PrevisaoTesteVizinhoMaisProximo)
PrecisaoModeloRedeNeural = accuracy_score(y_teste, PrevisaoTesteRedeNeural)
PrecisaoModeloMLP = accuracy_score(y_teste, PrevisaoTesteMLP)
PrecisaoModeloExtraTrees = accuracy_score(y_teste, PrevisaoTesteExtraTrees)

print("Precisao inicial do melhor modelo Arvore no conjunto de teste: ", PrecisaoModeloArvore)
print("Precisao inicial do melhor modelo Vizinho Mais Próximo no conjunto de teste: ", PrecisaoModeloVizinhoMaisProximo)
print("Precisao inicial do melhor modelo Rede Neural no conjunto de teste: ", PrecisaoModeloRedeNeural)
print("Precisao inicial do melhor modelo MLP no conjunto de teste: ", PrecisaoModeloMLP)
print("Precisao inicial do Melhor modelo Extra Trees no conjunto de teste: ", PrecisaoModeloExtraTrees)

# Avalia os melhores modelos com base no conjunto de validação
PrevisaoValidacaoModeloArvore = MelhorModeloArvore.predict(x_validacao)
PrevisaoValidacaoVizinhoMaisProximo = MelhorModeloVizinhoMaisProximo.predict(x_validacao)
PrevisaoValidacaoRedeNeural = MelhorModeloRedeNeural.predict(x_validacao)
PrevisaoValidacaoMLP = MelhorModeloMLP.predict(x_validacao)
PrevisaoValidacaoExtraTrees = MelhorModeloExtraTrees.predict(x_validacao)

# Avaliação da precisão no conjunto de validação
PrecisaoValidacaoModeloArvore = accuracy_score(y_validacao, PrevisaoValidacaoModeloArvore)
PrecisaoValidacaoVizinhoMaisProximo = accuracy_score(y_validacao, PrevisaoValidacaoVizinhoMaisProximo)
PrecisaoValidacaoRedeNeural = accuracy_score(y_validacao, PrevisaoValidacaoRedeNeural)
PrecisaoValidacaoMLP = accuracy_score(y_validacao, PrevisaoValidacaoMLP)
PrecisaoValidacaoExtraTrees = accuracy_score(y_validacao, PrevisaoValidacaoExtraTrees)

print("Precisao final do melhor modelo Arvore no conjunto de validacao: ", PrecisaoValidacaoModeloArvore)
print("Precisao final do melhor modelo Vizinho Mais Próximo no conjunto de validacao: ", PrecisaoValidacaoVizinhoMaisProximo)
print("Precisao final do melhor modelo Rede Neural no conjunto de validacao: ", PrecisaoValidacaoRedeNeural)
print("Precisao final do melhor modelo MLP no conjunto de validacao: ", PrecisaoValidacaoMLP)
print("Precisao final do Melhor modelo Extra Trees no conjunto de validacao: ", PrecisaoValidacaoExtraTrees)

# Crie um dicionário com os nomes dos modelos e suas respectivas precisões
modelos_precisoes = {
    'ModeloArvore': PrecisaoModeloArvore,
    'ModeloVizinhoMaisProximo': PrecisaoModeloVizinhoMaisProximo,
    'ModeloRedeNeural': PrecisaoModeloRedeNeural,
    'ModeloMLP': PrecisaoModeloMLP,
    'ModeloExtraTrees': PrecisaoModeloExtraTrees
}

# Encontre o modelo com a melhor precisão
MelhorModelo = max(modelos_precisoes, key=modelos_precisoes.get)

print("O melhor modelo foi: ", MelhorModelo)
print("Precisao do melhor modelo: ", modelos_precisoes[MelhorModelo])



#Variável utilizada no primeiro looping para que o mesmo seja executado 2 vezes
x=0

#Looping do código inteiro
while True:
    #Tempo até que o proximo looping seja executado
    time.sleep(5)
    #Looping da requisição incial das informações
    while True:
        #Requisição GET do Firebase
        requisicao=requests.get("https://projetolees-default-rtdb.firebaseio.com/.json")
        #Descobre o código de retorno da requisição GET
        Retorno = requisicao.status_code

        if Retorno == 200:
            #Define que a requisição das informações foi bem sucedida
            print("A requisicao funcionou.")

        else:
            #Define que a requisição das informações foi mal sucedida
            print("A requisicao nao funcionou. Tentando novamente...")
            #Tenta realizar a requisição novamente após 5 segundos
            time.sleep(5)
            continue


        
        #Adquirindo as infomarações  
        Lista= requisicao.json()

        #Atribuição dos valores do Firebase ao Pyton
        #D para a distância medida pelos sensores
        #Frequencia é a frequencia lida pelos sensores
        D1 = Lista['DistanciaS1']
        D2 = Lista['DistanciaS2']
        Frequencia1 = Lista["Frequencia1"]
        Frequencia2 = Lista["Frequencia2"]
        Frequencia3 = Lista["Frequencia3"]
        Frequencia4 = Lista["Frequencia4"]



        #Check-in para conferir se a solicitação das informações do Firebase foi bem sucedida
        if D1 == 22 and D2 == 22 and Frequencia1 == 7500 and Frequencia2 == 7500 and Frequencia3 == 7500 and Frequencia4 == 7500:
            print("A informacao dos sensores ainda nao foi atualizada pelo Arduino. Esperando...")
            #Espera 5 segundos antes de executar o looping novamente
            time.sleep(5)
        else:
            break


    #Executa a IA
    #Tenta prever qual será a saída correspondente a um novo cojunto de entradas utilizando o modelo de maior precisão
    Dados={
    "Frequencia1" : Frequencia1,
    "Frequencia2" : Frequencia2,
    "Frequencia3" : Frequencia3,
    "Frequencia4" : Frequencia4
    }

    x_uso = pd.DataFrame([Dados])

    Predict = MelhorModelo.predict(x_uso)

    Plastico, Papel, Metal, Vidro, Indefinido = Predict[0]

    #MATERIAL é o valor que a rede neural retorna
    #infoMATERIAL é o formato da sting Json
    #NewInfoMATERIAL é utilizado para adicionar o valor da variável python em infoMATERIAL
    #NewMATERIAL é o comando utilizado para enviar o valor definitivo ao Firebase

    print(Plastico)
    print(Papel)
    print(Metal)
    print(Vidro)
    print(Indefinido)
    

    Indefinido=int(Indefinido)
    Metal=int(Metal)
    Vidro=int(Vidro)
    Plastico=int(Plastico)
    Papel=int(Papel)


    if Plastico == 0 and Papel == 0 and Vidro == 0 and Metal == 0 and Indefinido == 0:
        Indefinido = 1
        Metal = 0
        Vidro = 0
        Plastico = 0
        Papel = 0


    #Adicionando os valores das variáveis nas strings Json 
    infoIndefinido = {"MaterialIndefinido" : Indefinido}
    NewInfoIndefinido = json.dumps(infoIndefinido)
    infoPlastico = {"MaterialPlastico" : Plastico}
    NewInfoPlastico = json.dumps(infoPlastico)
    infoPapel = {"MaterialPapel" : Papel}
    NewInfoPapel = json.dumps(infoPapel)
    infoMetal = {"MaterialMetal" : Metal}
    NewInfoMetal = json.dumps(infoMetal)
    infoVidro = {"MaterialVidro" : Vidro}
    NewInfoVidro = json.dumps(infoVidro)

    #Define que as informações de saída dos sensores é 0.
    infoD1 = '{"DistanciaS1" : 22}'
    infoD2 = '{"DistanciaS2" : 22}'
    infoFrequencia1 = '{"Frequencia1" : 7500}'
    infoFrequencia2 = '{"Frequencia2" : 7500}'
    infoFrequencia3 = '{"Frequencia3" : 7500}'
    infoFrequencia4 = '{"Frequencia4" : 7500}'

    #Looping da edição final das informações
    while True:
        #Modificando as informações do Firebase sobre o material descartado
        NewIndefinido= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=NewInfoIndefinido)
        NewPlastico= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=NewInfoPlastico)
        NewPapel= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=NewInfoPapel)
        NewMetal= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=NewInfoMetal)
        NewVidro= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=NewInfoVidro)
        #Modificando as informações do Firebase sobre a saída dos sensores
        NewD1= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=infoD1)
        NewD2= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=infoD2)
        NewFrequencia1= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=infoFrequencia1)
        NewFrequencia2= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=infoFrequencia2)
        NewFrequencia3= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=infoFrequencia3)
        NewFrequencia4= requests.patch("https://projetolees-default-rtdb.firebaseio.com/.json",data=infoFrequencia4)

        
        #Descobrindo se o envio das informações de saída da rede neural foi bem sucedido
        RetornoNewIndefinido = NewIndefinido.status_code
        RetornoNewPlastico = NewPlastico.status_code
        RetornoNewPapel = NewPapel.status_code
        RetornoNewMetal = NewMetal.status_code
        RetornoNewVidro = NewVidro.status_code
        #Descobrindo se a edição dos valores de saída dos sensores foi bem sucedida
        RetornoNewD1 = NewD1.status_code
        RetornoNewD2 = NewD2.status_code
        RetornoNewFrequencia1 = NewFrequencia1.status_code
        RetornoNewFrequencia2 = NewFrequencia2.status_code
        RetornoNewFrequencia3 = NewFrequencia3.status_code
        RetornoNewFrequencia4 = NewFrequencia4.status_code


        #Check-in para conferir se a modificação das informações do Firebase foi bem sucedida 
        if (RetornoNewD1 == 200 and RetornoNewD2 == 200 and RetornoNewFrequencia1 == 200 and RetornoNewFrequencia2 == 200 and RetornoNewFrequencia3 == 200 and RetornoNewFrequencia4 == 200 and RetornoNewIndefinido == 200 and RetornoNewMetal == 200 and RetornoNewPapel == 200 and RetornoNewPlastico == 200 and RetornoNewVidro == 200 ):
            print("A edicao das informacoes funcionou.")
            break

        else:
            print("A edicao das informacoes nao funcionou.")
            time.sleep(5)