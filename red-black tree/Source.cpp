#include <malloc.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

struct StromRB {
	int data;
	int barva; //0 - black, 1 - red
	struct StromRB *levy;
	struct StromRB *pravy;
	struct StromRB *otec;
};

static struct StromRB *root;

static StromRB *Create(int data) {
	StromRB *akt = (StromRB*)malloc(sizeof(StromRB));
	akt->data = data;
	akt->barva = 1;
	akt->levy = akt->pravy = akt->otec = NULL;
	return akt;
}

static StromRB *Find(StromRB *koren, int data) {
	StromRB *akt = koren;
	if (akt->data < data && akt->pravy != NULL) akt = Find(akt->pravy, data);
	else if (akt->data > data && akt->levy != NULL) akt = Find(akt->levy, data);
	akt = (akt != NULL && akt->data == data ? akt : NULL);
	return akt;
}

int BarvaTest(StromRB *koren, int syn) {
	if (syn && (koren->otec->otec->levy != NULL && koren->otec->otec->levy->barva)) return 1;
	else if (!syn && koren->otec->otec->pravy != NULL && koren->otec->otec->pravy->barva) return 1;
	return 0;
}

void RotaceLevo_new(StromRB *koren, int test) {
	StromRB *pom_d, *pom_o, *pom_s; //dedecek,otec a syn
	pom_d = koren->otec->otec;
	pom_o = koren->otec;
	pom_s = koren;
	if (pom_d == NULL) pom_s->otec = pom_d;
	else (pom_d->levy == pom_o ? pom_d->levy = pom_s : pom_d->pravy = pom_s);
	pom_s->otec = pom_d;
	pom_o->pravy = pom_s->levy;
	if (pom_s->levy != NULL) pom_s->levy->otec = pom_o;
	pom_s->levy = pom_o;
	pom_o->otec = pom_s;
	if (test == 1) {
		pom_s->barva = 0;
		(pom_s->pravy->barva ? pom_s->levy->barva = pom_s->pravy->barva = 1 : pom_s->levy->barva = pom_s->pravy->barva = 0);
	}
	if (pom_s->otec == NULL) root = pom_s;
}

void RotacePravo_new(StromRB *koren, int test) {
	StromRB *pom_d, *pom_o, *pom_s; //dedecek,otec a syn
	pom_d = koren->otec->otec;
	pom_o = koren->otec;
	pom_s = koren;
	if (pom_d == NULL) pom_s->otec = pom_d;
	else (pom_d->levy == pom_o ? pom_d->levy = pom_s : pom_d->pravy = pom_s);
	pom_s->otec = pom_d;
	pom_o->levy = pom_s->pravy;
	if (pom_s->pravy != NULL) pom_s->pravy->otec = pom_o;
	pom_s->pravy = pom_o;
	pom_o->otec = pom_s;
	if (test) {
		pom_s->barva = 0;
		(pom_s->levy->barva ? pom_s->levy->barva = pom_s->pravy->barva = 1 : pom_s->levy->barva = pom_s->pravy->barva = 0);
	}
	if (pom_s->otec == NULL) root = pom_s;
}
// 1.1 
void del_1(StromRB *koren) {
	int test;
	if (koren->otec == koren->otec->otec->levy) {
		test = (koren == koren->otec->pravy ? 1 : 0);
		switch (test)
		{
		case 1:
			RotaceLevo_new(koren, 0);
			//break;
		case 0:
			if (!test) RotacePravo_new(koren->otec, 0);
			else RotacePravo_new(koren, 0);
			break;
		}
	}
	else {
		test = (koren == koren->otec->levy ? 1 : 0);
		switch (test)
		{
		case 1:
			RotacePravo_new(koren, 0);
			//break;
		case 0:
			if (!test) RotaceLevo_new(koren->otec, 0);
			else RotaceLevo_new(koren, 0);
			break;
		}
	}
	koren->barva = 1; // koren misto uzla pom_s
	koren->pravy->barva = koren->levy->barva = 0; // koren misto uzla pom_s
}
// 1.2
void del_2(StromRB *koren) {
	koren->otec->barva = 0;
	koren->barva = 1;
}
//2.1.1
void del_3(StromRB *koren, int syn) {
	int test;
	if (koren == koren->otec->levy) { // minus 1n uroven (otec->)
		test = (syn ? 1 : 0);
		switch (test)
		{
		case 1:
			RotaceLevo_new(koren->pravy, 0);
			//break;
		case 0:
			if (!test) RotacePravo_new(koren->otec, 0);
			else RotacePravo_new(koren, 0);
			break;
		}
		if (koren->levy->pravy != NULL) koren->levy->pravy->barva = 0;
		if (koren->levy->levy != NULL) koren->levy->levy->barva = 0;
	}
	else {
		test = (syn ? 0 : 1);
		switch (test)
		{
		case 1:
			RotacePravo_new(koren->levy, 0);
			//break;
		case 0:
			if (!test) RotaceLevo_new(koren->otec, 0);
			else RotaceLevo_new(koren, 0);
			break;
		}
		if (koren->pravy->pravy != NULL) koren->pravy->pravy->barva = 0;
		if (koren->pravy->levy != NULL) koren->pravy->levy->barva = 0;
	}
}
//2.1.2
void del_4(StromRB *koren) {
	StromRB *pom_d, *pom_o, *pom_s; //dedecek,otec a syn
	pom_d = koren->otec->otec;
	pom_o = koren->otec;
	pom_s = koren;
	if (pom_d != NULL) (pom_d->levy == pom_o ? pom_d->levy = pom_s : pom_d->pravy = pom_s);
	pom_s->otec = pom_d;
	if (pom_s == pom_o->levy) {
		pom_o->levy = pom_s->pravy;
		pom_s->pravy->otec = pom_o;
		pom_s->pravy = pom_o;
	}
	else {
		pom_o->pravy = pom_s->levy;
		pom_s->levy->otec = pom_o;
		pom_s->levy = pom_o;
	}
	pom_o->otec = pom_s;
	pom_s->barva = 0;
	if (pom_o->levy != NULL) pom_o->levy->barva = 1;
	if (pom_o->pravy != NULL) pom_o->pravy->barva = 1;
}
//2.2.1
void del_5(StromRB *koren) {
	int test;
	if (koren->otec == koren->otec->otec->levy) {
		test = (koren == koren->otec->pravy ? 1 : 0);
		switch (test)
		{
		case 1:
			RotaceLevo_new(koren, 0);
			//break;
		case 0:
			if (!test) RotacePravo_new(koren->otec, 0);
			else RotacePravo_new(koren, 0);
			break;
		}
	}
	else {
		test = (koren == koren->otec->levy ? 1 : 0);
		switch (test)
		{
		case 1:
			RotacePravo_new(koren, 0);
			//break;
		case 0:
			if (!test) RotaceLevo_new(koren->otec, 0);
			else RotaceLevo_new(koren, 0);
			break;
		}
	}
	koren->barva = koren->pravy->barva = koren->levy->barva = 0;
}
//2.2.2
void del_6(StromRB *koren) {
	koren->barva = 1;
}
// z DelOprav preposilame do funkce del_n uzel b (odecetl jsem otca v kazde funkci del_n)
void DelOprav(StromRB *koren) {
	StromRB *akt = koren->otec, *tmp = koren, *b_akt;// tmp - uzel, ktery mazeme; b_akt - uzel odlisny od tmp
	b_akt = (akt->levy == tmp ? akt->pravy : akt->levy);
	if (akt != NULL) {
		if (akt->barva) ((b_akt->levy != NULL && b_akt->levy->barva) || (b_akt->pravy != NULL && b_akt->pravy->barva) ? del_1(b_akt) : del_2(b_akt));
		else {
			if (b_akt != NULL && b_akt->barva) {
				if (b_akt->levy != NULL) {
					if ((b_akt->levy->levy != NULL && b_akt->levy->levy->barva) || (b_akt->levy->pravy != NULL && b_akt->levy->pravy->barva)) del_3(b_akt, 0);
				}
				else if (b_akt->pravy != NULL) {
					if ((b_akt->pravy->levy != NULL && b_akt->pravy->levy->barva) || (b_akt->pravy->pravy != NULL && b_akt->pravy->pravy->barva)) del_3(b_akt, 1);
				}
				else del_4(b_akt);
			}
			else {
				if ((b_akt->levy != NULL && b_akt->levy->barva) || (b_akt->pravy != NULL && b_akt->pravy->barva)) del_5(b_akt);
				else del_6(b_akt);
			}
		}
	}
}

static StromRB *Min(StromRB *akt) {
	if (akt->levy == NULL) return akt;
	return Min(akt->levy);
}

void Del_new(StromRB *koren, int data) {
	StromRB *akt, *tmp;
	akt = Find(koren, data);
	if (akt != NULL) {
		if (akt->pravy == NULL) (akt->levy != NULL ? (tmp = akt->levy, akt->levy->otec = akt->otec, akt = akt->levy) : (tmp = akt, akt->otec->levy = NULL));
		else {
			tmp = Min(akt->pravy);
			akt->data = tmp->data;
			if (tmp != NULL && !tmp->barva) {
				if (tmp->levy == NULL && tmp->pravy == NULL) DelOprav(tmp);
				else (tmp->levy == NULL ? tmp->pravy->barva = 0 : tmp->levy->barva = 0);
			}
			if (tmp->otec->pravy == tmp) {
				tmp->otec->pravy = tmp->pravy;
				if (tmp->pravy != NULL) tmp->pravy->otec = tmp->otec;
			}
			else tmp->otec->levy = NULL;
		}
	}
}

static StromRB *InsrtOprav(StromRB *koren) {
	int test;
	if (koren->otec->barva) {
		if (koren->otec == koren->otec->otec->levy) {
			if (BarvaTest(koren, 0)) {
				koren->otec->barva = koren->otec->otec->pravy->barva = 0;
				koren->otec->otec->barva = 1;
				if (koren->otec->otec->otec != NULL)InsrtOprav(koren->otec->otec);
			}
			else {
				test = (koren == koren->otec->pravy ? 1 : 0);
				switch (test)
				{
				case 1:
					RotaceLevo_new(koren, 0);
					//break;
				case 0:
					if (!test) RotacePravo_new(koren->otec, 1);
					else RotacePravo_new(koren, 1);
					break;
				}
			}
		}
		else {
			if (BarvaTest(koren, 1)) {
				koren->otec->barva = koren->otec->otec->levy->barva = 0;
				koren->otec->otec->barva = 1;
				if (koren->otec->otec->otec != NULL)InsrtOprav(koren->otec->otec);
			}
			else {
				test = (koren == koren->otec->levy ? 1 : 0);
				switch (test)
				{
				case 1:
					RotacePravo_new(koren, 0);
					//break;
				case 0:
					if (!test) RotaceLevo_new(koren->otec, 1);
					else RotaceLevo_new(koren, 1);
					break;
				}
			}
		}
	}
	if (root->barva) root->barva = 0;
	if (koren != NULL && koren->otec != NULL && koren->otec->barva) InsrtOprav(koren);
	else return koren;
}

static StromRB *Insert(StromRB *koren, int data) {
	StromRB *akt = koren, *tmp = Create(data);
	if (koren == NULL) {
		tmp->barva = 0;
		root = koren = tmp;
		return tmp;
	}
	while (akt != NULL) {
		if (akt->data <= data) {
			if (akt->pravy == NULL) {
				akt->pravy = tmp;
				akt->pravy->otec = akt;
				akt = InsrtOprav(akt->pravy);
				return koren;
			}
			else akt = akt->pravy;
		}
		else {
			if (akt->levy == NULL) {
				akt->levy = tmp;
				akt->levy->otec = akt;
				akt = InsrtOprav(akt->levy);
				return koren;
			}
			else akt = akt->levy;
		}
	}
	return koren;
}

static void Print(StromRB *koren, int hloubka) {
	int i;
	if (koren == NULL) return;
	for (i = 0; i < hloubka; i++) printf("*");
	printf("%d (%s)\n", koren->data, (koren->barva ? "Red" : "Black"));
	Print(koren->levy, hloubka + 1);
	Print(koren->pravy, hloubka + 1);
}

static StromRB *CreatePole(StromRB *koren, int pole[], int length) {
	for (int i = 0; i < length; i++)
		koren = Insert(root, pole[i]);
	return koren;
}

int main() {
	StromRB *koren = NULL;
	int test = 0;
	int pole[] = { 20,30,10,15,13,5,12,7,15,6,8,79,45,75,98,20,31,25,67,8 };
	//int pole[] = { 20,30,15,35 };
	koren = CreatePole(koren, pole, sizeof(pole) / sizeof(int));
	Print(koren, 0);
	printf("------\n\n");
	Del_new(koren, 30);
	Del_new(koren, 13);
	Del_new(koren, 10);
	Del_new(koren, 45);
	Del_new(koren, 20);
	Del_new(koren, 79);
	Print(koren, 0);
	free(koren);
	getch();
	return 0;
}