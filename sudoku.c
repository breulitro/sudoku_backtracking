#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int arrependimentos = 0;
int tentativas = 0;

int mat[9][9];
int debug = 0;

void dump_matriz() {
	int i, j;

	system("clear");
	printf("-------------------------------------------------------\n");
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (((j + 1) % 3) == 0)
				printf(" (%d)  | ", mat[i][j]);
			else if (j == 0)
				printf("|  (%d) ", mat[i][j]);
			else
				printf(" (%d) ", mat[i][j]);
		}
		puts("");
		if (((i + 1) % 3) == 0)
			printf("-------------------------------------------------------\n");
	}
}

int pode_linha(int v, int l) {
	int i;

	for (i = 0; i < 9; i++) {
		if (mat[l][i] == v) {
			return 0;
		}
	}
	return 1;
}
int pode_coluna(int v, int c) {
	int i;

	for (i = 0; i < 9; i++) {
		if (mat[i][c] == v)
			return 0;
	}
	return 1;
}

int pode_quadrante(int v, int c, int l) {
	int c1, l1;
	int i, j;

	// Pedala robinho
	c1 = (c / 3) * 3;
	l1 = (l / 3) * 3;

	for (i = l1; i < l1 + 3; i++)
		for (j = c1; j < c1 + 3; j++)
			if (mat[i][j] == v)
				return 0;
	return 1;
}

int pode(int v, int l, int c) {
	tentativas++;
	return pode_linha(v, l) && pode_coluna(v, c) && pode_quadrante(v, c, l);
}

int preenche(int l, int c) {
	int i, j, v;
	int ai, aj;

	if (mat[l][c] != 0) {
		if (c == 8)
			return preenche(l + 1, 0);
		else
			return preenche(l, c + 1);
	}

	for (i = l; i < 9; i++) {
		for (j = c; j < 9; j++) {
			for (v = 1; v <= 9; v++) {
				if (pode(v, i, j)) {
					mat[i][j] = v;
					if (debug)
						dump_matriz();
					if (j == 8) {
						ai = i + 1;
						aj = 0;
					} else {
						ai = i;
						aj = j + 1;
					}

					if (!preenche(ai, aj)) {
						// Me arrependi
						mat[i][j] = 0;
						if (debug)
							dump_matriz();
						arrependimentos++;
					} else {
						return 1;
					}
				}
			}
			return 0;
		}
	}

	return 1;
}

void le_matriz(char *file) {
	int l, i;
	char lmat[9][9];
	FILE *fd;
	char *buf, *tok;
	int size;

	memset(mat, 0, 81);

	buf = NULL;

	if (file != NULL) {
		fd = fopen(file, "r");
		fseek(fd, 0, SEEK_END);
		size = ftell(fd);
		fseek(fd, 0, SEEK_SET);
		buf = malloc(size);
		fread(buf, size, 1, fd);
		fclose(fd);
		tok = buf;
		for (l = 0; l < 9; l++) {
			memcpy(lmat[l], strtok_r(tok, "\n", &tok), 9);
		}
	} else {
		for (l = 0; l < 9; l++) {
			printf("linha %d:", l + 1);
			scanf("%s", lmat[l]);
		}
	}

	for (l = 0; l < 9; l++)
		for (i = 0; i < 9; i++)
			if (lmat[l][i] != '.')
				mat[l][i] = lmat[l][i] - 0x30;

	if (buf != NULL)
		free(buf);
}

int main(int argc, char **argv) {
	char op;
	char *file = NULL;

	while ((op = getopt(argc, argv, "df:")) != -1) {
		switch (op) {
			case 'f':
				file = optarg;
				break;
			case 'd':
				debug = 1;
				break;
			default:
				printf("Unknown option\n");
		}
	}

	le_matriz(file);
	dump_matriz();
	printf("\n\n");
	preenche(0, 0);
	dump_matriz();

	printf("%d tentativas\n", tentativas);
	printf("%d arrependimentos\n", arrependimentos);
}
