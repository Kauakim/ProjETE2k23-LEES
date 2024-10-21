//Adiciona as bibliotecas que serão utilizadas pelo ESP32
//Wifi.h é utilizada para gerenciar a conexão Wifi do ESP32
//HTTPClient.h é utilizada para realizar requisições HTTP pelo ESP32
//ArduinoJson.h é utilizada para trabalhar com formatos do tipo Json com o ESP32
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>


//==============================================================================================================================================


//Define o nome e senha da rede atual
const char* ssid = "RedeKwanzas";
const char* password = "kaua6170";


//==============================================================================================================================================


const int SensorMagnetico = (34); //Pino do sensor magnético do projeto

const int PinoFrequencia1 = (14); //Pino do sensor de capacitância com base em frequência
const int PinoFrequencia2 = (12); //Pino do sensor de capacitância com base em frequência
const int PinoFrequencia3 = (26); //Pino do sensor de capacitância com base em frequência
const int PinoFrequencia4 = (13); //Pino do sensor de capacitância com base em frequência

const int echoPin1 = (27); //HC-SR04 ECHO 1 (Recebe o sinal ultrassônico anteriormente enviado)
const int trigPin1 = (33); //HC-SR04 TRIG 1 (Envia o sinal para o sensor ultrassônico)
const int echoPin2 = (32); //HC-SR04 ECHO 2 (Recebe o sinal ultrassônico anteriormente enviado)
const int trigPin2 = (25); //HC-SR04 TRIG 2 (Envia o sinal para o sensor ultrassônico)

const int Motor1AH = (23); //Motor do deslocamento da caixa (Anti Horário)
const int Motor1H = (19); //Motor do deslocamento da caixa (Horário)
const int Motor2AH = (18); //Motor do deslocamento da caixa (Anti Horário)
const int Motor2H = (4); //Motor do deslocamento da caixa (Horário)


//====================================================================================================================================================


float FrequenciaMediaInicial1, FrequenciaMediaInicial2, FrequenciaMediaInicial3, FrequenciaMediaInicial4; //Frequência média do sensor capacitivo (10000 valores)
int DistanciaS1, DistanciaS2; //Distância dos sensores HC-SR04
float Tempo1, Tempo2; //Tempo gasto pelos sensores HC-SR04 da saída do sinal até o retorno
float FrequenciaProcessada1, FrequenciaProcessada2, FrequenciaProcessada3, FrequenciaProcessada4; //Frequência media após o processamento de Base-Media
float FrequenciaMedida1, FrequenciaMedida2, FrequenciaMedida3, FrequenciaMedida4; //Frequência lida no pino do sensor capacitivo oscilador, quando o mesmo estiver cheio
float FrequenciaBase1, FrequenciaBase2, FrequenciaBase3, FrequenciaBase4; //Frequência media lida no pino do sensor capacitivo oscilador, quando o mesmo estiver vazio


float ComecoCiclo1, FimCiclo1, Periodo1, FrequenciaInstantanea1, SomaTudoInicial1, SomaTudoFinal1; //Variáveis para calcular o período do sinal do sensor capacitivo 1
float ComecoCiclo2, FimCiclo2, Periodo2, FrequenciaInstantanea2, SomaTudoInicial2, SomaTudoFinal2; //Variáveis para calcular o período do sinal do sensor capacitivo 2
float ComecoCiclo3, FimCiclo3, Periodo3, FrequenciaInstantanea3, SomaTudoInicial3, SomaTudoFinal3; //Variáveis para calcular o período do sinal do sensor capacitivo 3
float ComecoCiclo4, FimCiclo4, Periodo4, FrequenciaInstantanea4, SomaTudoInicial4, SomaTudoFinal4; //Variáveis para calcular o período do sinal do sensor capacitivo 4

const int x = 2500; //Tamanho das primeiras listas
float ListaFrequencia1[2500], ListaFrequencia2[2500], ListaFrequencia3[2500], ListaFrequencia4[2500];

//Variáveis de repetição do código
int a; //Looping da Lista ListaFrequencia
int VariavelAnalise;  //Looping para correção de valores inf dos sensores capacitivos


//====================================================================================================================================================


int Indefinido, Metal, Vidro, Plastico, Papel; //Material correspondente ao resíduo descartado
int TotalIndefinido, TotalMetal, TotalVidro, TotalPlastico, TotalPapel; //Quantidade de lixo total descartado até o momento de cada tipo

int httpResponseCode; //Variável utilizada para fazer as comunicações HTTP do ESP32

int ContadorMedido, ContadorDesejado; //Variáveis de contagem do deslocamento em relação ao número de reed-switches

int LeituraSensor;


//===================================================================================================================================================


//Comandos que serão executados uma única vez
void setup()
{
  //Inicia o monitor serial do ESP32
  Serial.begin(115200);

  Serial.println("Pinos declarados");

  pinMode(SensorMagnetico, INPUT);

  pinMode(PinoFrequencia1, INPUT);
  pinMode(PinoFrequencia2, INPUT);
  pinMode(PinoFrequencia3, INPUT);
  pinMode(PinoFrequencia4, INPUT);

  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);

  pinMode(Motor1H, OUTPUT);
  pinMode(Motor1AH, OUTPUT);
  pinMode(Motor2H, OUTPUT);
  pinMode(Motor2AH, OUTPUT);

  //Tenta se conectar ao Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi.");
}


//==============================================================================================================================================


//Coleta os valores das saídas da IA e do total de lixo descartado até o momento de cada tipo
void GetValores()
{
  //Inicia uma instância HTTP no ESP32
  HTTPClient http;
  String url = "https://projetolees-default-rtdb.firebaseio.com/.json";
  http.begin(url);

  //Realiza uma requisição HTTP GET ao banco de dados do Firebase
  httpResponseCode = http.GET();

  //Confere o código de retorno da requisição HTTP, e atribue as informações as variàveis escolhidas
  if (httpResponseCode > 0)
  {
    //Exibe na tela a informação coletada do Firebase
    String payload = http.getString();
    Serial.println("Resposta do Firebase:");
    Serial.println(payload);

    //Inicia o buffer Json do ESP32
    DynamicJsonDocument doc(1024);

    //Realiza o parsing do JSON recebido
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
      Serial.print("Falha ao fazer parsing do JSON: ");
      Serial.println(error.c_str());
    }
    else
    {
      //Atribue os valores retornados em JSON nas variáveis do ESP32
      Indefinido = doc["MaterialIndefinido"];
      Metal = doc["MaterialMetal"];
      Vidro = doc["MaterialVidro"];
      Plastico = doc["MaterialPlastico"];
      Papel = doc["MaterialPapel"];
      TotalIndefinido = doc["TotalIndefinido"];
      TotalMetal = doc["TotalMetal"];
      TotalVidro = doc["TotalVidro"];
      TotalPlastico = doc["TotalPlastico"];
      TotalPapel = doc["TotalPapel"];

      Serial.print("Indefinido: ");
      Serial.println(Indefinido);
      Serial.print("Metal: ");
      Serial.println(Metal);
      Serial.print("Vidro: ");
      Serial.println(Vidro);
      Serial.print("Plastico: ");
      Serial.println(Plastico);
      Serial.print("Papel: ");
      Serial.println(Papel);

      Serial.print("TotalIndefinido: ");
      Serial.println(TotalIndefinido);
      Serial.print("TotalMetal: ");
      Serial.println(TotalMetal);
      Serial.print("TotalVidro: ");
      Serial.println(TotalVidro);
      Serial.print("TotalPlastico: ");
      Serial.println(TotalPlastico);
      Serial.print("TotalPapel: ");
      Serial.println(TotalPapel);
    }
  }
  else
  {
    //Exibe o código de erro no monitor serial
    Serial.print("Erro na solicitação GET HTTP: ");
    Serial.println(httpResponseCode);
  }
}


//==============================================================================================================================================


//Atualiza as informações de entrada da IA pelo Firebase
void AtualizarInicial()
{
  //Inicia uma instância HTTP no ESP32
  HTTPClient http;
  String url = "https://projetolees-default-rtdb.firebaseio.com/.json";
  http.begin(url);

  //Prepara o JSON com os valores das variáveis atualizadas
  DynamicJsonDocument doc(1024);
  //Atribue o valor desejado a variável escolhida
  doc["DistanciaS1"] = DistanciaS1;
  doc["DistanciaS2"] = DistanciaS2;
  doc["Frequencia1"] = FrequenciaProcessada1;
  doc["Frequencia2"] = FrequenciaProcessada2;
  doc["Frequencia3"] = FrequenciaProcessada3;
  doc["Frequencia4"] = FrequenciaProcessada4;

  //Serializa o JSON em uma string possível de ser enviada por HTML
  String jsonStr;
  serializeJson(doc, jsonStr);

  //Configura a solicitação HTTP em um fomato PATCH
  http.addHeader("Content-Type", "application/json");
  httpResponseCode = http.sendRequest("PATCH", jsonStr);

  if (httpResponseCode == HTTP_CODE_OK)
  {
    Serial.println("As variáveis foram atualizadas com sucesso no Firebase.");
  }
  else
  {
    //Exibe o código de erro no monitor serial
    Serial.print("Erro na solicitação PATCH HTTP: ");
    Serial.println(httpResponseCode);
  }

  // Finaliza a requisição HTTP PATCH
  http.end();
}


//==============================================================================================================================================


//Atualiza todas as informações possíveis do Firebase, resetando os valores que forem necessários
void AtualizarFinal()
{
  //Inicia uma instância HTTP no ESP32
  HTTPClient http;
  String url = "https://projetolees-default-rtdb.firebaseio.com/.json";
  http.begin(url);

  //Prepara o JSON com os valores das variáveis atualizadas
  DynamicJsonDocument doc(1024);
  //Atribue o valor desejado a variável escolhida
  doc["DistanciaS1"] = 22;
  doc["DistanciaS2"] = 22;
  doc["Frequencia1"] = 7500;
  doc["Frequencia2"] = 7500;
  doc["Frequencia3"] = 7500;
  doc["Frequencia4"] = 7500;
  doc["MaterialIndefinido"] = 0;
  doc["MaterialMetal"] = 0;
  doc["MaterialVidro"] = 0;
  doc["MaterialPlastico"] = 0;
  doc["MaterialPapel"] = 0;
  doc["TotalIndefinido"] = TotalIndefinido;
  doc["TotalMetal"] = TotalMetal;
  doc["TotalVidro"] = TotalVidro;
  doc["TotalPlastico"] = TotalPlastico;
  doc["TotalPapel"] = TotalPapel;

  //Serializa o JSON em uma string possível de ser enviada por HTML
  String jsonStr;
  serializeJson(doc, jsonStr);

  //Configura a solicitação HTTP em um fomato PATCH
  http.addHeader("Content-Type", "application/json");
  httpResponseCode = http.sendRequest("PATCH", jsonStr);

  if (httpResponseCode == HTTP_CODE_OK)
  {
    Serial.println("As variáveis foram atualizadas com sucesso no Firebase.");
  }
  else
  {
    //Exibe o código de erro no monitor serial
    Serial.print("Erro na solicitação PATCH HTTP: ");
    Serial.println(httpResponseCode);
  }

  // Finaliza a requisição HTTP PATCH
  http.end();
}


//==============================================================================================================================================


//Abre e fecha o alçapão da caixa
void Descarte()
{
  //Desativa os motores do projeto, por segurança
  digitalWrite(Motor2H, 0);
  digitalWrite(Motor2AH, 0);
  digitalWrite(Motor1H, 0);
  digitalWrite(Motor1AH, 0);

  //Tempo de espera, para evitar sobrecarga de corrente no motor
  delay(500);

  //Abre o alçapão
  digitalWrite(Motor2H, 1);
  delay(333);
  digitalWrite(Motor2H, 0);

  //Tempo de espera, para evitar sobrecarga de corrente no motor
  delay(2500);

  //Fecha o alçapão
  digitalWrite(Motor2AH, 1);
  delay(5000);
  digitalWrite(Motor2AH, 0);

  //Tempo de espera, para evitar sobrecarga de corrente no motor
  delay(500);
}


//==============================================================================================================================================


//Define o número de reed-switches do projeto com base no material descartado e descobre o número total de lixos descartados até o momento
void DefinirContador()
{
  if (Indefinido == 1)
  {
    ContadorDesejado = 1;
    TotalIndefinido = TotalIndefinido + 1;
  }

  else if (Metal == 1)
  {
    ContadorDesejado = 2;
    TotalMetal = TotalMetal + 1;
  }

  else if (Vidro == 1)
  {
    ContadorDesejado = 3;
    TotalVidro = TotalVidro + 1;
  }

  else if (Plastico == 1)
  {
    ContadorDesejado = 4;
    TotalPlastico = TotalPlastico + 1;
  }

  else if (Papel == 1)
  {
    ContadorDesejado = 5;
    TotalPapel = TotalPapel + 1;
  }
}


//==============================================================================================================================================


void AtribuicaoMagnetica()
{
  LeituraSensor = analogRead(SensorMagnetico);
  Serial.print("Leitura do sensor magnetico: ");
  Serial.println(LeituraSensor);

  if (LeituraSensor >= 900 && LeituraSensor <= 1200)
  {
    ContadorMedido = 5;
  }
  else if (LeituraSensor >= 1850 && LeituraSensor <= 2000)
  {
    ContadorMedido = 4;
  }
  else if (LeituraSensor >= 2500 && LeituraSensor <= 2700)
  {
    ContadorMedido = 3;
  }
  else if (LeituraSensor >= 2550 && LeituraSensor <= 2800)
  {
    ContadorMedido = 2;
  }
  else if (LeituraSensor >= 3800 && LeituraSensor <= 4100)
  {
    ContadorMedido = 1;
  }
}


//==============================================================================================================================================


//Desloca a caixa para a lixeira correspondente ao material descartado
void Deslocamento()
{
  ContadorMedido = 0;

  Serial.println("Desligando os motores da LEES.");

  //Desativa os motores do projeto, por segurança
  digitalWrite(Motor2H, 0);
  digitalWrite(Motor2AH, 0);
  digitalWrite(Motor1H, 0);
  digitalWrite(Motor1AH, 0);


  //Descobre o valor do contador necessário de reed-switches
  DefinirContador();

  AtribuicaoMagnetica();
  //Avança até a lixeira desejada
  while (ContadorMedido != ContadorDesejado)
  {
    AtribuicaoMagnetica();
    /*
    Serial.print("Leitura do sensor magnetico: ");
    Serial.println(LeituraSensor);
    */
    Serial.print("Valor do contador magnético desejado: ");
    Serial.println(ContadorDesejado);
    Serial.print("Valor do contador magnético lido: ");
    Serial.println(ContadorMedido);

    digitalWrite(Motor1AH, 1);
    delay(750);
    digitalWrite(Motor1AH, 0);
    //Tempo de espera, para segurança
    delay(500);

    Serial.println("Indo até o destino");
  }

  Serial.println("Chegou até o destino");

  //Ao chegar na lixeira desejada, descarta o resíduo no local correto
  Descarte();

  Serial.println("Descarte realizado");

  //Retorna até a primeira lixeira
  AtribuicaoMagnetica();
  while (ContadorMedido != 1)
  {
    AtribuicaoMagnetica();
    Serial.print("Leitura do sensor magnetico: ");
    Serial.println(LeituraSensor);
    Serial.print("Valor do contador magnético desejado: ");
    Serial.println(ContadorDesejado);
    Serial.print("Valor do contador magnético lido: ");
    Serial.println(ContadorMedido);

    digitalWrite(Motor1H, 1);
    delay(2000);
    digitalWrite(Motor1H, 0);
    //Tempo de espera, para segurança
    delay(500);

    Serial.println("Deslocando até o início.");
  }
  
  Serial.println("Chegou até o início.");
}


//==============================================================================================================================================


//Descobre o valor da frequência do sensor capacitivo oscilador
void Frequencia()
{
  SomaTudoInicial1 = 0;
  SomaTudoInicial2 = 0;
  SomaTudoInicial3 = 0;
  SomaTudoInicial4 = 0;

  for (a = 0; a < x; a = a + 1)
  {
    ComecoCiclo1 = pulseIn(PinoFrequencia1, HIGH);
    FimCiclo1 = pulseIn(PinoFrequencia1, LOW);

    ComecoCiclo2 = pulseIn(PinoFrequencia2, HIGH);
    FimCiclo2 = pulseIn(PinoFrequencia2, LOW);

    ComecoCiclo3 = pulseIn(PinoFrequencia3, HIGH);
    FimCiclo3 = pulseIn(PinoFrequencia3, LOW);

    ComecoCiclo4 = pulseIn(PinoFrequencia4, HIGH);
    FimCiclo4 = pulseIn(PinoFrequencia4, LOW);

    //Tempo do ciclo em micro segundos (T)
    Periodo1 = ComecoCiclo1 + FimCiclo1;
    Periodo2 = ComecoCiclo2 + FimCiclo2;
    Periodo3 = ComecoCiclo3 + FimCiclo3;
    Periodo4 = ComecoCiclo4 + FimCiclo4;

    //Frequencia do sinal em hertz (f)
    FrequenciaInstantanea1 = 1000000.00 / Periodo1;
    ListaFrequencia1[a] = FrequenciaInstantanea1;
    FrequenciaInstantanea2 = 1000000.00 / Periodo2;
    ListaFrequencia2[a] = FrequenciaInstantanea2;
    FrequenciaInstantanea3 = 1000000.00 / Periodo3;
    ListaFrequencia3[a] = FrequenciaInstantanea3;
    FrequenciaInstantanea4 = 1000000.00 / Periodo4;
    ListaFrequencia4[a] = FrequenciaInstantanea4;

    SomaTudoInicial1 = SomaTudoInicial1 + ListaFrequencia1[a];
    SomaTudoInicial2 = SomaTudoInicial2 + ListaFrequencia2[a];
    SomaTudoInicial3 = SomaTudoInicial3 + ListaFrequencia3[a];
    SomaTudoInicial4 = SomaTudoInicial4 + ListaFrequencia4[a];
  }

  FrequenciaMediaInicial1 = SomaTudoInicial1 / x;
  FrequenciaMediaInicial2 = SomaTudoInicial2 / x;
  FrequenciaMediaInicial3 = SomaTudoInicial3 / x;
  FrequenciaMediaInicial4 = SomaTudoInicial4 / x;
}


//==============================================================================================================================================


//Descobre os valores das distâncias dos sensores ultrassônicos HC-SR04
void Distancias()
{
  //Envio do sinal para o primeiro HC-SR04
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  //Recibemento do sinal do sensor e cálculo da distância medida pelo primeiro HC-SR04
  Tempo1 = pulseIn(echoPin1, HIGH);
  DistanciaS1 = (Tempo1 * 0.034) / 2;

  //Envio do sinal para o segundo HC-SR04
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  //Recibemento do sinal do sensor e cálculo da distância medida pelo primeiro HC-SR04
  Tempo2 = pulseIn(echoPin2, HIGH);
  DistanciaS2 = (Tempo2 * 0.034) / 2;

  delay(500);
}


//==============================================================================================================================================


//Analisa a caixa para ver se já possui algum resíduo ou não
void Analise()
{
  //Medindo as tensões dos sensores HC-SR04
  Frequencia();
  //Medindo as distâncias dos sensores HC-SR04
  Distancias();
}


//==============================================================================================================================================


//Analisa a informação coletada pelos sensores, para que nenhum dos valores lidos estejam estranhos para a IA do projeto
void AnaliseCorrigida()
{
  //Analisa a informação dos sensores, até que não haja nenhum erro nos valores lidos pelos sensores
  VariavelAnalise = 1;

  while (VariavelAnalise == 1)
  {
    Analise();

    if (FrequenciaMediaInicial1 >= 500000 || FrequenciaMediaInicial2 >= 500000 || FrequenciaMediaInicial3 >= 500000 || FrequenciaMediaInicial4 >= 500000)
    {
      Serial.println("A medicao das frequencias não funcionou, coloque o lixo novamente, ou retire-o");

      /*
      Serial.println(FrequenciaMediaInicial1);
      Serial.println(FrequenciaMediaInicial2);
      Serial.println(FrequenciaMediaInicial3);
      Serial.println(FrequenciaMediaInicial4);
      */
      
      VariavelAnalise = 1;
    }
    else
    {
      Serial.println("A medicao das frequencias funcionou completamente, concluindo o descarte");

      /*
      Serial.println(FrequenciaMediaInicial1);
      Serial.println(FrequenciaMediaInicial2);
      Serial.println(FrequenciaMediaInicial3);
      Serial.println(FrequenciaMediaInicial4);
      */
      
      VariavelAnalise = 0;
    }

    /*
    Serial.print(DistanciaS1);
    Serial.print(",     ");
    Serial.println(DistanciaS2);
    */
  }
}


//==============================================================================================================================================



//Comandos que serão executados repetidamente
void loop()
{
  //Desativa os motores do projeto, por segurança
  digitalWrite(Motor2H, 0);
  digitalWrite(Motor2AH, 0);
  digitalWrite(Motor1H, 0);
  digitalWrite(Motor1AH, 0);


  FrequenciaBase1 = 0;
  FrequenciaBase2 = 0;
  FrequenciaBase3 = 0;
  FrequenciaBase4 = 0;

  
  AnaliseCorrigida();
  while (FrequenciaBase1 == 0 && FrequenciaBase2 == 0 && FrequenciaBase3 == 0 && FrequenciaBase4 == 0)
  {
    AnaliseCorrigida();
    
    if(DistanciaS2 >= 16 && DistanciaS2 <= 20)
    {
      FrequenciaBase1 = FrequenciaMediaInicial1;
      FrequenciaBase2 = FrequenciaMediaInicial2;
      FrequenciaBase3 = FrequenciaMediaInicial3;
      FrequenciaBase4 = FrequenciaMediaInicial4;
    }
  }

  Serial.println("Frequencia de base coletada");


  Serial.print("Frequencia base 1: ");
  Serial.print(FrequenciaBase1);
  Serial.print(" , Frequencia base 2: ");
  Serial.print(FrequenciaBase2);
  Serial.print(" , Frequencia base 3: ");
  Serial.print(FrequenciaBase3);
  Serial.print(" , Frequencia base 4: ");
  Serial.print(FrequenciaBase4);


  //Coleta a informação da capacitância padrão dos sensores capacitivos e confere o interior da caixa, até que a mesma não fique vazia
  AnaliseCorrigida();
  while (DistanciaS2 >= 16 && DistanciaS2 <= 20)
  {
    AnaliseCorrigida();
    Serial.println("Ative o descarte.");
  }

  Serial.print("Frequencia Media Inicial 1: ");
  Serial.print(FrequenciaMediaInicial1);
  Serial.print(" , Frequencia Media Inicial 2: ");
  Serial.print(FrequenciaMediaInicial2);  
  Serial.print(" , Frequencia Media Inicial 3: ");
  Serial.print(FrequenciaMediaInicial3);
  Serial.print(" , Frequencia Media Inicial 4: ");
  Serial.print(FrequenciaMediaInicial4);

  //Envia somente o diferencial de frequência para a IA
  FrequenciaProcessada1 = FrequenciaBase1 - FrequenciaMediaInicial1;
  FrequenciaProcessada2 = FrequenciaBase2 - FrequenciaMediaInicial2;
  FrequenciaProcessada3 = FrequenciaBase3 - FrequenciaMediaInicial3;
  FrequenciaProcessada4 = FrequenciaBase4 - FrequenciaMediaInicial4;

  Serial.println("A operação com as frequências ja foi realizada.");

  Serial.print("FrequenciaProcessada 1: ");
  Serial.print(FrequenciaProcessada1);
  Serial.print(" , FrequenciaProcessada 2 : ");
  Serial.println(FrequenciaProcessada2);
  Serial.print("FrequenciaProcessada 3: ");
  Serial.print(FrequenciaProcessada3);
  Serial.print(" , FrequenciaProcessada 4: ");
  Serial.print(FrequenciaProcessada4);
  

  //Atualiza as entradas da IA no Firebase
  AtualizarInicial();


  //Tempo necessário para que a rede neural do projeto analise o material e atualize as informações do Firebase
  delay(5000);


  //Coleta as informações do Firebase sobre as saídas da IA e a quantidade total de lixos descartados de cada tipo
  GetValores();

  if (Indefinido == 0 && Metal == 0 && Vidro == 0 && Plastico == 0 && Papel == 0)
  {
    Indefinido = 1;
    Metal = 0;
    Vidro = 0;
    Plastico = 0;
    Papel = 0;
  }

  //Movimenta a caixa até a lixeira correspondente, descarta o lixo e volta a caixa até a posição inicial
  Deslocamento();


  //Confere o interior da caixa, até que a mesma fique vazia
  AnaliseCorrigida();
  while (DistanciaS2 <= 16 || DistanciaS2 >= 20)
  {
    AnaliseCorrigida();
    Deslocamento();
  }


  //Atualiza todas as informações possíveis do Firebase, resetando algumas delas aos valores iniciais
  AtualizarFinal();


  Serial.println("Ultima atualiazacao concluida no Firebase.");
  Serial.println("Seu lixo for descartado, volte sempre.");
  delay(5000);
}
