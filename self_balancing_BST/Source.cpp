#include <stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>

#pragma region define all my functions first

struct nod;
void preordine(nod *rad);
void introducere(nod *rad, nod *p);
void adauga(int valoare);
void freeArb(nod *rad);
void show();
int binaryTreeHeight(nod *p);
int inaltime(nod *p);
int returnMax(int a, int b);

void afiseaza_solutie(int n);
int acceptabil(int pos, int k);
int solutie(int k, int n);
void permutari(int k, int n);

//AVL stuff
int height(struct nod *N);
int maximBetween(int a, int b);
struct nod* newNode(int cheie);
struct nod *rotateRight(struct nod *y);
struct nod *rotateLeft(struct nod *x);
int getBalance(struct nod *N);
struct nod* add(struct nod* node, int val);

//define stuff here
int a[50], n;
int direction[100], directionSize = 0;
int direction2[100], directionSize2 = 0;

#pragma endregion


//my node
struct nod
{
	int valoare;
	struct nod *stang, *drept;
	int ech;
};
nod *radacina;


#pragma region AVL process

struct nod* newNode(int cheie)
{
	struct nod* node = (struct nod*)
		malloc(sizeof(struct nod));
	node->valoare = cheie;
	node->stang = NULL;
	node->drept = NULL;
	node->ech = 1;
	return(node);
}

struct nod* add(struct nod* node, int val)
{
	if (node == NULL)
		return(newNode(val));

	if (val < node->valoare)
		node->stang = add(node->stang, val);
	else if (val > node->valoare)
		node->drept = add(node->drept, val);
	else
		return node;


	node->ech = 1 + maximBetween(height(node->stang),
		height(node->drept));

	int balance = getBalance(node);

	if (balance > 1 && val < node->stang->valoare)
		return rotateRight(node);

	if (balance < -1 && val > node->drept->valoare)
		return rotateLeft(node);

	if (balance > 1 && val > node->stang->valoare)
	{
		node->stang = rotateLeft(node->stang);
		return rotateRight(node);
	}


	if (balance < -1 && val < node->drept->valoare)
	{
		node->drept = rotateRight(node->drept);
		return rotateLeft(node);
	}


	return node;
}

struct nod *rotateLeft(struct nod *x)
{
	struct nod *y = x->drept;
	struct nod *T2 = y->stang;

	y->stang = x;
	x->drept = T2;

	x->ech = maximBetween(height(x->stang), height(x->drept)) + 1;
	y->ech = maximBetween(height(y->stang), height(y->drept)) + 1;

	return y;
}

struct nod *rotateRight(struct nod *y)
{
	struct nod *x = y->stang;
	struct nod *T2 = x->drept;

	x->drept = y;
	y->stang = T2;

	y->ech = maximBetween(height(y->stang), height(y->drept)) + 1;
	x->ech = maximBetween(height(x->stang), height(x->drept)) + 1;

	return x;
}

int getBalance(struct nod *N)
{
	if (N == NULL)
		return 0;
	return height(N->stang) - height(N->drept);
}

int height(struct nod *N)
{
	if (N == NULL)
		return 0;
	return N->ech;
}

int maximBetween(int a, int b)
{
	return (a > b) ? a : b;
}

#pragma endregion

#pragma region find the height

int returnMax(int a, int b)
{
	if (a < b) return b;
	return a;
}

int inaltime(nod *p)
{
	if (p == NULL) return 0;
	return returnMax(inaltime(p->stang), inaltime(p->drept)) + 1;
}

int binaryTreeHeight(nod *p)
{
	return inaltime(p);
}

#pragma endregion

#pragma region show and free

void show()
{
	int height;
	height = binaryTreeHeight(radacina);
	preordine(radacina);
	freeArb(radacina);
	radacina = NULL;
	printf(" are inaltimea: %d", height);
}

void preordine(nod *rad)
{
	if (rad != NULL)
	{
		printf("%d ", rad->valoare);
		preordine(rad->stang);
		preordine(rad->drept);
	}
}

void freeArb(nod *rad)
{
	if (rad != NULL)
	{
		if (rad->drept != NULL)freeArb(rad->drept);
		if (rad->stang != NULL)freeArb(rad->stang);
		free(rad);
	}
}

#pragma endregion

#pragma region add (binary process)

void introducere(nod *rad, nod *p)
{
	if (rad != NULL)
	{
		if (p->valoare < rad->valoare)
		{
			if (rad->stang != NULL) introducere(rad->stang, p);
			else rad->stang = p;
		}

		else if (p->valoare > rad->valoare)
		{
			if (rad->drept != NULL) introducere(rad->drept, p);
			else rad->drept = p;
		}

		else printf("EROARE: AI ADAUGAT 2 NODURI IDENTICE (%d)\n", p->valoare);
	}
}

void adauga(int valoare)
{
	nod *p;
	p = (nod*)malloc(sizeof(nod));

	p->valoare = valoare;
	p->stang = NULL;
	p->drept = NULL;

	if (radacina == NULL) {
		radacina = p;
	}
	else
	{
		introducere(radacina, p);
	}
}

#pragma endregion

#pragma region compare bin trees

void countPreordine(nod *rad)
{
	if (rad != NULL)
	{
		direction[directionSize] = 1;
		directionSize++;
		countPreordine(rad->stang);

		direction[directionSize] = 2;
		directionSize++;
		countPreordine(rad->drept);
	}
	else
	{
		direction[directionSize] = 0;
		directionSize++;
	}
}

int equalitybetweenTrees()
{
	int i;
	printf("\n");
	for (i = 0; i < directionSize2; i++) printf("%d", direction2[i]);
	printf(" (debugging)\n");
	for (i = 0; i < directionSize; i++) printf("%d", direction[i]);
	printf(" (debugging)\n");

	if (directionSize != directionSize2)return 0;

	for (i = 0; i < directionSize; i++)
	{
		if (direction[i] != direction2[i])return 0;
	}

	return 1;
}

#pragma endregion

#pragma region backtrack

//de aici avem backtrack cautare / aflam permutarile recursiv
void afiseaza_solutie(int n)
{
	printf("\n(input)Testing purposes: ");
	//aici facem adaugare binar
	int i;
	for (i = 1; i <= n; i++)
	{
		printf("%d ", a[i]);
		adauga(a[i]);
	}
	printf("\nArborele binar: ");

	//save directions of the binary stuff
	directionSize = 0;
	countPreordine(radacina);

	//save one direction into the empty one
	directionSize2 = directionSize;

	for (i = 0; i < directionSize; i++)
		direction2[i] = direction[i];

	//show the binary
	show();

	//this is AVL
	for (i = 1; i <= n; i++)
	{
		radacina = add(radacina, a[i]);
	}
	printf("\nArborele binar echilibrat: ");

	//we save directions of AVL
	directionSize = 0;
	countPreordine(radacina);

	//show AVL
	show();

	//check the equality
	printf("Sunt egale: %d", equalitybetweenTrees());

	//make some space
	printf("\n\n");
}

int acceptabil(int pos, int k)
{
	int i;
	for (i = 0; i < k; i++)
		if (a[i] == pos)
			return 0;
	return 1;
}

int solutie(int k, int n)
{
	return (k == n);
}

void permutari(int k, int n)
{
	int i;
	for (i = 1; i <= n; i++)
	{
		a[k] = i;
		if (acceptabil(i, k))if (solutie(k, n)) afiseaza_solutie(n);
		else permutari(k + 1, n);
	}
}

#pragma endregion


int main()
{
	printf("insert number of nodes = ");
	scanf_s("%d", &n);

	permutari(1, n);
	return 0;
}
