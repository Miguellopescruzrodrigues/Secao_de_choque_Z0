# Cálculo da eficiência de identificação do múon

**Introdução**

Este é um exemplo simples do cálculo da eficiência de identificação do múon utilizando o método de ** TagAndProbe ** usando conjuntos de dados de CMS Open Data. Este projeto foi realizado utilizando um exemplo realizado pelo Allan Jales e Raphael Gomes de Souza sobre a orientação do professor Sandro fonseca.

https://github.com/allanjales/TagAndProbe/tree/systematic


**Estrutura**

Para acessar os dados deve-se utilizar o CMSSW, estrutura que possui o ambiente de análise. Para fazer isso, você tem duas maneiras:

 - **Usando Máquina Vitual**: Você deve baixar o [VM Virtual Box] (https://www.virtualbox.org/wiki/Download_Old_Builds). Certifique-se de que a versão baixada está funcionando com sua imagem CMS VM. Você pode ler mais [aqui] (http://opendata-dev.web.cern.ch/docs/cms-virtual-machine-2011). Você deve usar o CMS SHELL na versão CMS Open Data VM 1.5.1 como você pode ver [aqui] (http://opendata-dev.web.cern.ch/docs/cms-getting-started-2011). Com outras versões (CMS-OpenData-1.2.0 "ou" CMS-OpenData-1.3.0) você pode usar o terminal normal.


- **Usando Docker Container**: Se você está tendo problemas com a máquina virtual, pode usar o [Docker Container] (https://www.docker.com/resources/what-container). As instruções de instalação estão nesta [página] (https://docs.docker.com/install/). Como você pode ver na página, você pode buscar uma imagem CMSSW e criar e iniciar um contêiner usando o comando docker run `$ docker run --name opendata -it clelange / cmssw: 5_3_32 / bin / bash`, **mas com isso você não poderá usar o ROOT. Para fazer isso, você deve usar o comando:**

`$ sudo docker run --name {Nome do seu container} -it -e DISPLAY = $ DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix clelange / cmssw: 5_3_32 / bin / bash`

e você poderá usar ROOT. Para listar os contêineres que você criou, use o comando:

`$ sudo docker container ls -a`

Para abrir um container, basta usar o comando com o nome do container que você poderá ver a lista dos containers que você criou.

`$ sudo docker start <Nome_Container>`

`$ sudo docker attach <Nome_Container>`


Vamos criar a estrutura CMSSW. Neste exemplo, estamos usando **CMSSW_5_3_32**. Depois de emular as configurações de dados abertos do CMS com sucesso, você deve criar um ambiente CMSSW e mudar para o diretório `CMSSW_5_3_32 / src /` (observe que você usa o contêiner e não precisa usar os comandos abaixo):

`$ cmsrel CMSSW_5_3_32`

`$ cd CMSSW_5_3_32 / src /`

Inicie o ambiente de análise CMS.

`$ cmsenv`

Crie um diretório de trabalho para o analisador e vá para esse diretório

`$ mkdir TagAndProbeSample`


**Obtendo os dados**


Neste ponto, você deve copiar este Git para sua área usando o comando:

`$ git clone git@github.com:Miguellopescruzrodrigues/Secao_de_choque_Z0.git`

Vá para este diretório que você copiou do Git e compile o código:

`$ cd Secao_de_choque_Z0/Eficiencia-TagAndProbe`

`$ scram b`

Depois disso já é possível executar o código.

`$ cmsRun src/TagAndProbe2019.cc`

Aguarde e você verá um novo arquivo .root, chamado TagAndProbe.root. Você pode abrir este arquivo e ver seu conteúdo:

`$ root -l TagAndProbe_Z0_Data.root`

`$ new TBrownser`

Usou-se a ressonância do **Z0** neste exemplo para criar um arquivo .root com o mesmo conteúdo do método CMSSW **TagAndProbe**. Se você quiser estudar outra ressonância, basta abrir o arquivo de configuração (TagAndProbe_Model_Trigger.py) e modificar a faixa de massa na linha 46 e 47:

	minResonanceMass = cms.double(70.),# in GeV
	maxResonanceMass = cms.double(120.)# in GeV

Você pode alterar os arquivos do conjunto de dados na linha 15:

`$ "data/CMS_Run2011A_DoubleMu_AOD_12Oct2013-v1_10000_file_index.txt"`
	
e os triggers na linha 41:

	PathName = cms.untracked.vstring(["HLT_IsoMu24_eta2p1"]),



Já em posse de uma Ntuple contendo os as informações necessárias para calcular a eficiência de identificação do múon, mova seus dados e dirija-se para:
`$ mv TagAndProbe_Z0_Data efficiency_tools/fitting/tests`
`$ cd efficiency_tools/fitting/tests `

Onde se encontra um programa chamado **simplify_data.cpp**. Esse programa muda a organização do seu arquivo .root de modo que ele possa ser aplicado no código para calcular a eficiência 

`$ root -l simplify_data.cpp `

Com seu novo arquivo **Eficiencia_Z0_Data.root**, basta move-lo para a pasta DATA que se encontra o diretório anterior `$ cd ..`onde se está o **efficiency.cpp**(Código que realiza o calculo da eficiência).

`$ root -l efficiency.cpp `

É possível que ocorra algum problema na com os plots das eficiências caso a sua versão do root não esteja atualizada.

<div align="center">
<img src="https://user-images.githubusercontent.com/43889863/161596074-eb3ce7ae-e55c-4aa1-bef5-0635c2a91525.png" width="700px" />
</div>

<div align="center">
<img src="https://user-images.githubusercontent.com/43889863/161597071-2eef1120-9ad6-4181-aed6-6b7fa2f08cb2.png" width="700px" />
</div>

<div align="center">
<img src="https://user-images.githubusercontent.com/43889863/161598534-2045b388-98b5-4bd6-974f-a27e364983ac.png" width="700px" />
</div>

<div align="center">
<img src="https://user-images.githubusercontent.com/43889863/161597602-8a261f5f-89e2-4d6e-9d33-8d7972795e64.png" width="700px" />
</div>
