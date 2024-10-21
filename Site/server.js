//Importa as bibliotecas necessárias para a execução do código
//Express é utilizado para facilitar na criação de páginas e colocá-las no ar
//Path é utilizado para facilitar o caminho até os arquivos de código

const express = require('express');
const path = require('path');

//Importa toda a bilioteca express.js e a atribue na variável app
const app = express();

//Define a porta que será utilizada como localhost
const PORTA = 8081;

// Configura o diretório onde os arquivos estáticos estão localizados
app.use(express.static(__dirname));



app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'homepage.html'));
});

app.get('/login', (req, res) => {
    res.sendFile(path.join(__dirname, 'login.html'));
});

app.get('/graficos', (req, res) => {
    res.sendFile(path.join(__dirname, 'graficos.html'));
});

app.get('/terminal', (req, res) => {
    res.sendFile(path.join(__dirname, 'terminal.html'));
});

app.get('/xinas', (req, res) => {
    res.sendFile(path.join(__dirname, 'xinas.html'));
});

// Inicia o servidor localmente na porta escolhida
app.listen(PORTA, () => {
    console.log(`Servidor rodando em http://localhost:${PORTA}`);
});