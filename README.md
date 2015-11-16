# G3-bsiso
## G3
- Cristopher Morais
- Diogo Bernini
- Gunar Martins
- Joao Vitor

##Área: Gerenciamento de E/S
Objetivo: Análise, projeto e implementação de um device driver para kernel linux (x86).
Descrição: Este grupo deve projetar e implementar um device driver para o kernel Linux, plataforma x86, o qual será executado como módulo.
O device driver a ser implementado deve monitorar o teclado e, ao detectar uma determinada combinação de teclas, deve matar o processo cujo PID foi digitado após a combinação de tecla (ex. ctrl+alt 456).
Essa operação deve ocorrer de duas formas independentes: imediata (ao perceber a combinação de teclas definida e o PID) ou programada (tempo futuro, ex. ctrl+alt 456 4s). A definição dos parâmetros de entrada e valores de retorno faz parte da etapa de projeto do device driver.

## How to
Copiar pra pasta:

	drivers/misc/
Adicionar ao makefile da pasta:

	make -C ../../ SUBDIRS=$PWD modules
Acoplar modulo:

	sudo insmod ./killer.ko
Desacoplar módulo:

	sudo rmmod killer
Ver printk:

	dmesg | tail
