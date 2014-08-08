//
//  main.c
//  RSA em C do Ale
//
//  Created by Alessandro Camillo Gimenez de Menezes on 08/08/14.
//  Copyright (c) 2014 Alessandro Camillo Gimenez de Menezes. All rights reserved.
//
//  se o codigo for de alguma ajuda pra vc, me paga uns laches no intervalo ou as pinga em uns role ai que ja ta valendo

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


int primoAleatorio1000();
//funcao para "gerar" um numero aleatorio primo(numero divisivel apenas por 1 e por ele mesmo)
//entre 1000 e 9999
int primoAleatorio10();
//funcao para "gerar" um numero aleatorio primo(numero divisivel apenas por 1 e por ele mesmo)
//entre 10 e 99

int mdc(int num1, int num2);
//encontra o Maior Divisor Comum entre 2 numero atraves do algoritmo de Euclides visto em sala

bool primosEntreSi(int nConjunto, int numero);
//verifica recursivamente se 2 numeros(positivos) s„o primos entre si, isto È,
//se o maximo divisor comum entre eles È igual a 1

int * euclidesExtentido(int a, int b);
//Recebe dois inteiros nao negativos A e B e devolve um vetor cuja primeira posicao
//eh o mdc(A,B), a segunda posicao eh o valor U e a terceira o valor V tais que
//A*U + B*V = mdc(A,B)
//sendo que o terceiro valor, V, eh o inverso de B dentro do conjunto A

void limpaVetor(char *vetor, int tamanho); //apenas parar tirar o lixo de memoria
void limpaVetorCriptografado(char *vetor, int tamanho); //apenas parar tirar o lixo de memoria


char * criptografar(char * texto, int tamanho, int n, int chave);
//algoritmo para criptografar uma mensagem com base na chave publica
//todos os printf's contidos nessa funcao sao apenas para teste

char * descriptografar(char * texto, int tamanho, int n, int chave);
//algoritmo para criptografar uma mensagem com base na chave privada
//todos os printf's contidos nessa funcao sao apenas para teste


int main(int argc, char *argv[]) {
	
	srand(time(NULL));
	//criaÁ„o das chaves publicas e privadas
	int primo1 =  7; //primoAleatorio10();
	int primo2 =  11; //primoAleatorio10();
	printf("primo 1 =  %d \nprimo 2 = %d \n\n", primo1, primo2);
	//ester numeros deve ser ocultados, exibidos apenas a fim de teste
	
	int N = primo1*primo2;
	printf("N = primo1 * primo2 = %d * %d = %d\n", primo1, primo2, N);
	
	int totienteN = (primo1 - 1)*(primo2 - 1); //funÁ„o totiente (que nao deveria ser exibido)
	printf("totiente de N = %d\n", totienteN); //resultado exibido apenas a fim de teste
	
	int chavePublica;
	//sorteio de uma chave qualquer tal que  1 < chavePublica < totienteN
	do{
		chavePublica = rand()%totienteN +1;
	}while( !primosEntreSi(totienteN, chavePublica) || chavePublica == 1 );
	printf("numero chavePublica = %d \n", chavePublica);
	printf("\nportanto a chave publica completa sera:\nn = %d \nchavePublica = %d\n", N, chavePublica);
	
	//funÁ„o para extrair a chave privada
	int *vetor = euclidesExtentido(totienteN, chavePublica);
	//o ultimo elemento do vetor corresponde ao inverso de "chavePublica" no conjunto totienteN
	//que sera a nossa chave Privada, porem este elemento pode nao estar dentro do conjunto
	int chavePrivada = vetor[2];
	
	//traz devolta para o conjunto case seja maior que o conjunto
    while ( totienteN < chavePrivada ){
    	chavePrivada = chavePrivada - totienteN;
    }
    //traz devolta para o conjunto case seja negativo
	while(chavePrivada < 0){
		chavePrivada = chavePrivada + totienteN;
	}
	printf("\ninverso = %d = %d \n\n",vetor[2], chavePrivada);
	printf("portanto a chave privada completa sera:\nn = %d \nchavepPrivada = %d\n", N, chavePrivada);
	
	//loop para teste de criptografar e descriptografar
	int opcao, sair = 0;
	int tamanho = 25;
	//char mensagem [tamanho];
	char *mensagem = malloc(tamanho * sizeof(char));
	
	do{
		limpaVetor(mensagem, tamanho);
		printf("\n1) Criptografar mensagem");
		printf("\n2) Descriptografar mensagem");
		printf("\n3) Sair");
		printf("\no que deseja fazer? \n");
		scanf("%d%*c", &opcao);
        
		switch(opcao){
			case 1:
				printf("\ndigite a mensagem para criptografar(max %d caracteres): \n", tamanho);
				fgets(mensagem, tamanho, stdin);
				mensagem = criptografar(mensagem, tamanho, N,  chavePublica);
				
				break;
			case 2:
				printf("\ndigite a mensagem para descriptografar(max %d caracteres): \n", tamanho);
				fgets(mensagem, tamanho, stdin);
				mensagem = descriptografar(mensagem, tamanho, N,  chavePrivada);
                
				break;
			case 3:
				sair = 1;
				printf("\nterminando o programa.");
				break;
			default:
				printf("\nopcao invalida, escolha novamente.");
				break;
		}
		fflush(stdin);
        
	}while(sair != 1);
	
	printf("\n");
	system("pause");
	return 0;
}


char * criptografar(char *texto, int tamanho, int n, int chave){
	//c = m ^ chavePublica (mod n)
	int i, j;
	int auxiliar[tamanho];
	
    for(i = 0; i < tamanho; i++){
		printf("%c %d  %d \n", texto[i], texto[i], texto[i]%n);	 //mostra a letra e o respectivo valor ASCII
        //e o valor dentro do conjunto n
		auxiliar[i] = texto [i];
	}
	
	printf("\n");
	for(i = 0; auxiliar[i - 1] != 10; i++){
		printf("%d ", auxiliar[i]);
	}
	
	printf("\n");
	// c[i] = texto[i] ^ chavePublica (mod n)
	int C;
	for(i = 0; auxiliar[i] != 10 ; i++){ // 10 È o numero/character que indica o fim da mensagem no vetor
		C = 1; //elemento identidade
		if(  !(auxiliar[i] == 32) ){
			for(j = 0; j < chave; j++){
				C=C*auxiliar[i]%n;
			}
			auxiliar[i] = C; //verificado com auxilio deo wolfram alpha.
		}else{
			auxiliar[i] = 0;
		}
		
	}
	
	printf("\n");
	for(i = 0; auxiliar[i - 1] != 10; i++){
		printf("%d ", auxiliar[i]);
	}
	
	char *mensagem = malloc (tamanho * sizeof(char));
	limpaVetorCriptografado(mensagem, tamanho);
	printf("\n");
    
	for (i = 0; auxiliar[i] != 10 ; i++){ //tras todos characteres para dentro do s valores imprimiveis/digitaveis
		mensagem[i] = auxiliar[i] + 32;
		printf("%c", mensagem[i]);
	}
	printf("%c", mensagem[i]);
	printf("\n");
    // printf("%s \n", mensagem);
	return mensagem;
}

char * descriptografar(char * texto, int tamanho, int n, int chave){
	// m = c ^ chavePrivada (mod n)
	int i, j;
	int auxiliar[tamanho];
	for(i = 0; auxiliar[i] != 10; i++){
		auxiliar[i] = texto[i] - 32;
	}
    
	// texto[i] = c[i] ^ chavePrivada (mod n)
	int C;
	for(i = 0; auxiliar[i] != 10 ; i++){
		C = 1; //elemento identidade
		if(  !(auxiliar[i] == 0) ){
			for(j = 0; j < chave; j++){
				C=C*auxiliar[i]%n;
			}
			auxiliar[i] = C;
		}
		else{
			auxiliar[i] = 32;
		}
	}
	// nao funciona pois os numeros sao extremamente grandes para este computador calcular,
	// porem o procedimento algoritmico funciona. verificado com auxilio deo wolfram alpha.
	
    printf("\n");
    //funciona apenas para quando n = 77 ---------------
	for(i = 0; auxiliar[i] != 10; i++){
		if (  !(65 <= auxiliar[i] && auxiliar[i] < n) ){
			auxiliar[i] = auxiliar[i] + n;
		}
		printf("%d ", auxiliar[i]);
	}
	//-----------------------------------------------------
	
	char *mensagem = malloc (tamanho * sizeof(char));
	limpaVetor(mensagem, tamanho);
	
	printf("\n");
	for (i = 0; auxiliar[i] != 10; i++){
		mensagem[i] = auxiliar[i];
		printf("%c", mensagem[i]);
	}
	//printf("%s \n", mensagem);
	printf("\n");
	return mensagem;
}


int primoAleatorio1000(){
	int numero = rand()%8973 +1000;//sorteio de um numero aleatorio da ordem de 10^3 (entre 1000 e 9972)
    //ps: ate 9972 pois este algoritmo pega o maior primo mais proximo do
    //numero sorteado, que no caso se sair o 9972, sera o 9973.
    //se sorteasse o 9973, o proximo primo sera da ordem 10^4
	int tdivisores =1, divisor = 1;
	int resto;
	//algoritmo para a busca de um numero primo a partir do numero aleatorio
	while (numero != divisor){
		resto = numero%divisor;
		if(resto == 0){
			tdivisores++;
		}
		if(tdivisores == 3){
			numero++;
			divisor = 0;
			tdivisores = 1;
		}
		divisor++;
	}
	if(tdivisores == 2){
		return numero;
	}
	else{
		printf("nao foi possivel gerar o numero primo\n");
		return -1;
	}
}

int primoAleatorio10(){
	int numero = rand()%83 +10; //sorteio de um numero aleatorio da ordem de 10^1 (entre 10 e 92)
    //ps: ate 92 pois este algoritmo pega o maior primo mais proximo do
    //numero sorteado, que no caso se sair o 92, sera o 93.
    //se sorteasse o 93, o proximo primo sera da ordem 10^2
	int tdivisores =1, divisor = 1;
	int resto;
	//algoritmo para a busca de um numero primo a partir do numero aleatorio
	while (numero != divisor){
		resto = numero%divisor;
		if(resto == 0){
			tdivisores++;
		}
		if(tdivisores == 3){
			numero++;
			divisor = 0;
			tdivisores = 1;
		}
		divisor++;
	}
	if(tdivisores == 2){
		return numero;
	}
	else{
		printf("nao foi possivel gerar o numero primo\n");
		return -1;
	}
}

int mdc(int num1, int num2){
	int maior, menor, resto;
	if (num1 > num2){   //descobrindo qual dos numeros eh o maior pois nao ha garantia
		maior = num1;   //de que o primeiro parametro sempre seja maior que o segundo
		menor = num2;   //na chamada inicial
	}
	else{
		maior = num2;
		menor =  num1;
	}
	
	resto = maior%menor;
	switch (resto){
		case 0:
			return menor;
			break;
		case 1:
			return 1;
			break;
		default:
			return mdc(menor, resto);
			break;
	}
}

bool primosEntreSi(int nConjunto, int numero){
	if( mdc(nConjunto, numero) == 1 ){
		return true;
	}
	else{
		return false;
	}
}

int * euclidesExtentido(int a, int b){
	int r = a;
    int r1 = b;
    int u = 1;
    int v = 0;
    int u1 = 0;
    int v1 = 1;
    // vari·veis auxiliares para efetuar trocas
    int rs, us, vs, q;
    
    while (r1 != 0){
    	q = (int) (r / r1); // pega apenas a parte inteira
        rs = r;
        us = u;
        vs = v;
        r = r1;
        u = u1;
        v = v1;
        r1 = rs - q *r1;
        u1 = us - q*u;
        v1 = vs - q*v1;
    }
    int * resultado = malloc(3*sizeof(int));
    resultado[0] = r;
    resultado[1] = u;
    resultado[2] = v;
    return resultado; //tais que R = A*U + B*V
}

void limpaVetor(char *vetor, int tamanho){
	int i;
	for(i = 0; i < tamanho; i++){
		vetor[i] = 00;
	}
}

void limpaVetorCriptografado(char *vetor, int tamanho){
	int i;
	for(i = 0; i < tamanho; i++){
		vetor[i] = 00;
	}
}

