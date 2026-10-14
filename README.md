# Monitoramento de temperatura com ESP32

Projeto do Desafio 16 de Internet das Coisas da Católica de Santa Catarina. O ESP32 lê a temperatura do DHT22 a cada 2 segundos, acende o LED correspondente e mostra os dados no monitor serial. Também se conecta à rede simulada Wokwi-GUEST.

[Abrir o projeto no Wokwi](https://wokwi.com/projects/477891055977330689)

| Temperatura | Estado | LED |
| --- | --- | --- |
| Abaixo de 25 °C | Normal | Verde |
| De 25 °C a 30 °C | Atenção | Amarelo |
| Acima de 30 °C | Crítico | Vermelho |

Para testar, inicie a simulação, clique no DHT22 e altere a temperatura para 20, 28 e 35 °C. Confira o LED aceso e a leitura no monitor serial.

O código está em `sketch.ino`, a montagem em `diagram.json` e a biblioteca em `libraries.txt`. As capturas dos testes estão na pasta `evidencias`.
