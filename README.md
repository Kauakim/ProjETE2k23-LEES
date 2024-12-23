# LEES - Lixeira Eletrônica Econômica Sustentável
![Projeto Finalizado](https://img.shields.io/badge/Status-Finalizado-brightgreen)
![Licença Não Comercial](https://img.shields.io/badge/Licen%C3%A7a-N%C3%A3o--Comercial-red)

A **LEES** - Lixeira Eletrônica Econômica Sustentável - é um lixeira inteligente inovadora desenvolvida para a **ProjETE 2K23**, uma feira de projetos tecnológicos realizada anualmente pela **ETE FMC**, a primeira escola técnica da América Latina.
Projetada para promover a sustentabilidade e garantir um futuro mais digno e promissor para as futuras gerações, a LEES automatiza a separação de resíduos com base no material de cada item descartado, facilitando a reciclagem e garantindo o descarte adequado dos resíduos, contribuindo significativamente para a redução do impacto ambiental e reforçando o compromisso com um futuro mais sustentável.

---

## Funcionamento do Projeto  

O funcionamento da LEES pode ser descrito em três etapas principais:  

- **Análise**:  
  Nesta etapa, o projeto coleta informações sobre o resíduo descartado utilizando dois sensores ultrassônicos HC-SR04 e quatro sensores capacitivos desenvolvidos pela equipe com materiais recicláveis.  
  Os sensores ultrassônicos funcionam de forma semelhante à ecolocalização de morcegos, emitindo sons em frequências altas (40 kHz), inaudíveis para humanos, que retornam como eco ao colidirem com um objeto. O tempo entre a emissão e o retorno do som é usado pelo ESP-32, microcontrolador do projeto, para calcular a distância entre os sensores e o resíduo, determinando sua posição na caixa.  
  Paralelamente, os sensores capacitivos detectam alterações no campo elétrico próximo, semelhantes ao funcionamento de telas sensíveis ao toque. Desenvolvidos com um circuito similar a um multivibrador astável, esses sensores variam sua frequência de saída conforme a permissividade elétrica do material descartado, permitindo identificar o tipo de material do resíduo.  

- **Processamento**:  
  O ESP-32 realiza os cálculos necessários com os dados coletados pelos sensores e os organiza em uma única mensagem, enviada ao banco de dados do projeto, o Realtime Database do Firebase por meio de uma requisição HTTP.  
  Em seguida, o "cérebro" da LEES entra em ação. Um código executado em algum computador coleta essas informações do banco de dados a partir de outra requisição HTTP, processa os dados e os adapta ao formato necessário para a rede neural da lixeira, um modelo Random Forest Classifier do Scikit-Learn, responsável por analisar os dados recebidos e classifica o material descartado como metal, vidro, plástico, papel ou indefinido.  
  Por fim, a classificação é então devolvida ao banco de dados, permitindo que o ESP-32 acesse essa informação e direcione o lixo para a lixeira correspondente.  

- **Separação**:  
  Na etapa final do descarte, o projeto utiliza dois motores de vidro elétrico utilizados em automóveis, acionados por uma fonte ATX de 200W e controlados pelo ESP-32 com drivers ponte H desenvolvidos pela equipe.  
  Inicialmente, um dos motores desloca a caixa contendo o lixo pelo trilho interno da LEES até a lixeira correta, para que o outro motor, responsável por abrir e fechar a caixa, descarte o resíduo na lixeira correspondente (Metal, Vidro, Plástico, Papel ou Indefinido).
  Por fim, após o descarte, a caixa é fechada e retorna à posição inicial, pronta para receber novos resíduos.  

Além do processo principal de separação, a LEES também conta com um site que informa o usuário sobre o projeto, emite alertas caso alguma lixeira esteja cheia, identificadas por sensores infravermelhos posicionados sobre cada compartimento, e fornece dados sobre a quantidade de resíduos descartados de cada material até o momento.  

Para saber mais sobre o projeto, acesse o link do diário de bordo da LEES, documento desenvolvido ao longo de todo o processo de desenvolvimento do projeto pela equipe com fotos, vídeos, arquivos e versões dos códigos utilizados, ou assista ao vídeo gravado pelo grupo.
- [Diário de Bordo](https://eteacojeorg-my.sharepoint.com/:o:/g/personal/rosa_rafael_edu_etefmc_com_br/EkO5bkhdYG5FoNh1IA1nRyUB2ByUAueX1Sehp29Ed4tsqw?e=4kbgHL)
- [Vídeo](https://eteacojeorg-my.sharepoint.com/:v:/g/personal/kaua_ribeiro_edu_etefmc_com_br/EdsKwRQHleRKleLEk93FQxsBkLeLHDcyaQhE6t2bfR_1Nw?nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJTdHJlYW1XZWJBcHAiLCJyZWZlcnJhbFZpZXciOiJTaGFyZURpYWxvZy1MaW5rIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXcifX0%3D&e=2glZtT)

---

## Tecnologias Utilizadas

### Software:
- Inteligência Artificial: Modelo Random Forest Classifier do SKT-Learn
- Interface Web: Dashboard interativo
- Banco de Dados: Firebase (Realtime Database)

### Hardware:
- Processamento: ESP-32
- Sensoriamento:
  Sensores capacitivos feitos a partir de materiais recicláveis, desenvolvidos pelo grupo
  Sensores ultrassônicos
  Sensores infravermelho
- Atuadores: Motores de baixa rotação reciclados de sistemas de vidro elétrico de carros
- Alimentação: Fonte ATX

### Linguagens e Bibliotecas:
- Linguagens de Programação: Python, C++, HTML5, CSS3, JavaScript
- Bibliotecas e Frameworks:
  IA: Scikit-learn
  Análise de Dados: Pandas
  Banco de Dados: Firebase
- Protocolos e Tecnologias: HTTP 

---

## Fundadores

</br>
<div align="center">
  <img src="./Fundadores.png" alt="Foto do Grupo" width="600">
</div>
</br>

- **Samuel de Jesus Pereira**
- **Marianne Lima** 
- **Kauã Ribeiro** - Líder  
  [LinkedIn](https://www.linkedin.com/in/kaua-ribeiro17/) | [GitHub](https://github.com/Kauakim)  
- **Rafael Carlos**  

---

**Agradecimentos**  
O projeto contou com o apoio crucial da **ETE-FMC**, com orientação dos professores **Ana Letícia Gomes Gonçalves**, **Fábio Teixeira** e **José Manoel**.
