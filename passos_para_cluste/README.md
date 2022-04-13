# Passos que segui para processar os dados no cluster 

**acessando o cluste**

Estando conectado a rede T2_BR_UERJ ou conectado a internet diretamente da sala dos alunos do DFNAE acesso ao analysis6 da seguinte maneira:

`$ ssh -X -Y miguellopes@10.0.200.20`


**Configurando o Token**

Primeiro verificar a existência do diretório abaixo.

`$ ls -l ~/.condor/tokens.d`

Caso ele não exista, criar com os comandos abaixo.

`$ cd ~`
`$ mkdir .condor`
`$ cd .condor`
`$ mkdir tokens.d`

É nescessario requisitar um Token valido para a equipe da T2_BR_UERJ. Tendo o Token em mãos ele feve ser colocado colocado em um arquivo chamado TOKEN, logo faça:

`$ cd ~/.condor/tokens.d`
`& vim TOKEN`

Basta colar o seu Token no arquivo e salva-lo.

**Configurando um ambiente com a versão que eu uso do cmssw (CMSSW_5_3_32)**

Este comando ira criar um diretório chamado CMSSW_5_3_32 em que estará configurada a versão 5_3_32 do CMSSW.

`& cmsrel CMSSW_5_3_32`
Conferindo se o ambiente foi instalado corretamente:
`&  ls -lah CMSSW_5_3_32/`

- **Baixando os arquivos do git**
Pronto agora vamos baixar os arquivos que vamos rodar de um github, para isso vá até o diretório:

`$ cd CMSSW_5_3_32/src`

`$ git clone https://github.com/Miguellopescruzrodrigues/Secao_de_choque_Z0.git`


**Rodando código**

Vá para o diretório:

`$ cd Secao_de_choque_Z0/Eficiencia-TagAndProbe`

Não se esqueça de configurar o ambiente e fazer um executavel do programa fazendo:

`$ cmsenv`
`$ scram b`

Agora podemos rodar o nosso código.

`$ cmsRun configs/TagAndProbe_Data.py`

pronto, espero que tudo corra bem. =)
