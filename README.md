# TrabalhoFinal_FSE

**Aluno:** João Pedro Silva de Carvalho
**Matricula:** 180033743

**Aluno:** Erick Giffoni 
**Matricula:** 170141161

## Sobre:

- Foi utilizado o plataformIO para o desenvolvimento do firmware da ESP32.
- A versão do framework esp-idf é 4.3.2


## Como rodar:

### Servidor Distribuido:

Configura o wifi via menuconfig, usando o SSID e a senha.
Configura o servidor no menu Configuração MQTT que rodará na máquina onde o servidor central rodará 
(conecte a esp e a máquina onde o central irá rodar na mesma rede e use o ip)
    O host MQTT tem o seguinte formato:
```
ws://<ip_servidor_central>:8083/mqtt
```
Configura no menuconfig se a esp irá rodar em low-power ou não (0 para desativado e 1 para low power ativado)

### Servidor central 

Rode o seguinte comando:
```
docker-compose up
```
Obs: Certifique-se que o seu computador permite acesso de outros aparelhos da mesma rede na porta 8083 para a comunicação mqtt
