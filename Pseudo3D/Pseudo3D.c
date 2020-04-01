# include "Janela.h"
# include "input.h"
# define _CRT_SECURE_NO_WARNINGS
# define PI 3.1415

typedef struct { float x; float y;}vector2D;
typedef struct { vector2D v1; vector2D v2; } parede;
typedef struct { parede paredes[4]; } sector;
typedef struct { vector2D pos; float ang; Rect rect; Texture tex; } entidade;
typedef struct { vector2D pos; float ang; float FOV; } camera;

float moduloVetor(vector2D v1) {
	return sqrt((v1.x*v1.x) + (v1.y*v1.y));
}
float calcAngulo(vector2D v1, vector2D v2) {

	float result;

	result = acos(((v1.x*v2.x)+(v1.y*v2.y))/(sqrt((v1.x*v1.x)+(v1.y*v1.y))*sqrt((v2.x*v2.x) + (v2.y*v2.y))));

	return result;

}
float calcDistancia(vector2D pos1, vector2D pos2) {
	return sqrt(((pos2.x -pos1.x)* (pos2.x - pos1.x)) + ((pos2.y - pos1.y) * (pos2.y - pos1.y)));
}
vector2D vetorUnitario(vector2D vetor) {
	float modulo = sqrt((vetor.x * vetor.x) + (vetor.y * vetor.y));
	vector2D novo_vetor;

	novo_vetor.x = vetor.x / modulo;
	novo_vetor.y = vetor.y / modulo;

	return novo_vetor;

}
vector2D intersessao(vector2D p1, vector2D p2, vector2D p3, vector2D p4) {

	float A1, B1, C1;
	float A2, B2, C2;
	float A3, B3, C3;
	float X, Y;

	vector2D r;

	A1 = p1.y - p2.y;
	B1 = (p2.x - p1.x);
	C1 = (p1.x * p2.y) - (p1.y * p2.x);

	A2 = p3.y - p4.y;
	B2 = (p4.x - p3.x);
	C2 = (p3.x * p4.y) - (p3.y * p4.x);


	if (A1 * p1.x + B1 * p1.y + C1 == 0) {
		drawCircle(0, 0, 5, 1, 0, 0);
	}
	if (A1 * p2.x + B1 * p2.y + C1 == 0) {
		drawCircle(10, 0, 5, 1, 0, 0);
	}

	if (A2 * p3.x + B2 * p3.y + C2 == 0) {
		drawCircle(20, 0, 5, 1, 0, 0);
	}
	if (A2 * p4.x + B2 * p4.y + C2 == 0) {
		drawCircle(30, 0, 5, 1, 0, 0);
	}

	if (A2 == 0) {
		drawCircle(100, 0, 5, 1, 0.5, 0);
		B2 *= -1;
	}
	else if (B2 == 0) {
		drawCircle(110, 0, 5, 0, 1, 0);
		A3 = A1;
		B3 = B1;
		C3 = C1;

		A1 = A2;
		B1 = B2;
		C1 = C2;

		A2 = A3;
		B2 = -B3;
		C2 = C3;

	}
	C1 *= -1;
	C2 *= -1;

	Y = (C2 * A1 - A2 * C1) / (B1 * A2 - B2 * A1);
	X = (C1 - (B1 * Y)) / A1;

	r.x = X;
	r.y = Y;

	return r;
}

int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,float p2_x, float p2_y, float p3_x, float p3_y)
{
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	float s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		// Collision detected
		return 1;
	}

	return 0; // No collision
}
vector2D radToVector(float ang) {
	vector2D vetor;
	vetor.x = cos(ang);
	vetor.y= sin(ang);
	return vetor;
}
vector2D verticeToVector(vector2D v1, vector2D v2) {
	vector2D vetor;
	vetor.x = v2.x - v1.x;
	vetor.y = v2.y - v1.y;
	return vetor;
}
int visibilidadeEntidade(entidade ent, camera cam) {
	vector2D vetor, dir;
	float angulo;
	dir = radToVector(cam.ang);
	vetor.x = ent.pos.x - cam.pos.x;
	vetor.y = ent.pos.y - cam.pos.y;
	angulo = calcAngulo(vetor, dir);
	if (angulo > cam.FOV / 2) {
		return 0;
	}
	return 1;
}
void renderEntidade(entidade ent, camera cam, Janela janela,int alturaCam) {

	vector2D vetor, extremidade;
	float distancia, angulo;

	vetor.x = ent.pos.x - cam.pos.x;
	vetor.y = ent.pos.y -cam.pos.y;
	extremidade = radToVector(cam.ang+ (cam.FOV/2));
	distancia = calcDistancia(ent.pos, cam.pos);
	angulo = calcAngulo(vetor, extremidade);
	
	alturaCam *= 100 / distancia;
	ent.rect.W *= 100/distancia;
	ent.rect.H *= 100/distancia;
	ent.rect.X = (janela.largura - ((angulo / cam.FOV) * janela.largura)) - (ent.rect.W / 2);
	ent.rect.Y = ((janela.altura / 2) + alturaCam )-ent.rect.H;

	drawTexture(ent.tex,&ent.rect,NULL);
	
}
parede renderVertice(vector2D vertice, camera cam, Janela janela, int altura, int alturaCam) {
	float angulo, anguloDirecao, distancia;
	vector2D vetor, extremidade, direcao;
	parede linha;
	vetor.x = vertice.x - cam.pos.x;
	vetor.y = vertice.y - cam.pos.y;

	extremidade = radToVector(cam.ang - (cam.FOV / 2));
	direcao = radToVector(cam.ang);
	angulo = calcAngulo(extremidade, vetor);
	anguloDirecao = calcAngulo(direcao, vetor);
	distancia = calcDistancia(cam.pos, vertice);
	altura *= 100 / distancia;
	alturaCam *= 100 / distancia;

	linha.v1.x = linha.v2.x = (angulo / cam.FOV) * janela.largura;
	linha.v2.y = (janela.altura / 2) + (alturaCam);
	linha.v1.y = linha.v2.y - altura;
	drawLine(linha.v1.x, linha.v1.y, linha.v2.x, linha.v2.y, 1, 0.3, 0);
	return linha;

}
void renderParede(parede p, camera cam, Janela janela, int altura, int alturaCam) {
	parede canto1, canto2;
	canto1 = renderVertice(p.v1, cam, janela, altura,alturaCam);
	canto2 = renderVertice(p.v2, cam, janela, altura,alturaCam);
	drawLine(canto1.v1.x, canto1.v1.y, canto2.v1.x, canto2.v1.y, 0, 1, 0.5);
	drawLine(canto1.v2.x, canto1.v2.y, canto2.v2.x, canto2.v2.y, 0, 1, 0.5);

}
void renderSector(sector setor, camera cam, Janela janela, int altura, int alturaCam) {
	int i;
	for (i = 0; i < 4; i++) {
		vector2D vetorD, vetor1, vetor2, vetorNormal;
		float angulo1, angulo2;
		vetor1.x = setor.paredes[i].v1.x - cam.pos.x;
		vetor1.y = setor.paredes[i].v1.y - cam.pos.y;
		vetor2.x = setor.paredes[i].v2.x - cam.pos.x;
		vetor2.y = setor.paredes[i].v2.y - cam.pos.y;
		vetorD = radToVector(cam.ang);
		vetorNormal.x = (setor.paredes[i].v1.x + setor.paredes[i].v2.x) / 2;
		vetorNormal.y = (setor.paredes[i].v1.y + setor.paredes[i].v2.y) / 2;
		vetorNormal = verticeToVector(cam.pos, vetorNormal);


		angulo1 = calcAngulo(vetorD, vetor1);
		angulo2 = calcAngulo(vetorD, vetor2);

		vector2D extremidade1, extremidade2;
		int r1, r2;
		extremidade1 = radToVector(cam.ang - cam.FOV / 2);
		extremidade1.x += cam.pos.x;
		extremidade1.y += cam.pos.y;
		extremidade1.x *= 1000;
		extremidade1.y *= 1000;
		extremidade2 = radToVector(cam.ang + cam.FOV / 2);
		extremidade2.x += cam.pos.x;
		extremidade2.y += cam.pos.y;
		extremidade2.x *= 1000;
		extremidade2.y *= 1000;


		if (angulo1 < PI/2 || angulo2 < PI/2) {

			//se vejo 2 vertices
			if (angulo1 < cam.FOV / 2 && angulo2 < cam.FOV / 2) {
				renderParede(setor.paredes[i], cam, janela, altura, alturaCam);
			}


				// se só vejo um vertice
				else if (angulo1 < cam.FOV / 2 || angulo2 < cam.FOV / 2) {
					vector2D outVector, extremidade, newVertice;
					parede novaParede = setor.paredes[i];
					int verticeSelection;

					//procura qual vertice estou enchergando
					if (angulo1 < cam.FOV / 2) {
						verticeSelection = 2;
						outVector = vetor2;
					}
					else {
						verticeSelection = 1;
						outVector = vetor1;
					}

					
					if (calcAngulo(radToVector(cam.ang + PI / 2), outVector) > PI / 2) {
						// direita
						extremidade = radToVector(cam.ang - cam.FOV / 2);
						extremidade.x += cam.pos.x;
						extremidade.y += cam.pos.y;
					}
					else {
						//esquerda
						extremidade = radToVector(cam.ang + cam.FOV / 2);
						extremidade.x += cam.pos.x;
						extremidade.y += cam.pos.y;
					}
					newVertice = intersessao(cam.pos, extremidade, setor.paredes[i].v1, setor.paredes[i].v2);
					newVertice.x += radToVector(cam.ang).x;
					newVertice.y += radToVector(cam.ang).y;

					if (calcAngulo(vetorD, verticeToVector(cam.pos, newVertice)) > PI / 2)
					{

						if (calcAngulo(radToVector(cam.ang + PI / 2), outVector) > PI / 2) {
							// direita
							extremidade = radToVector(cam.ang + cam.FOV / 2);
							extremidade.x += cam.pos.x;
							extremidade.y += cam.pos.y;
						}
						else {
							//esquerda
							extremidade = radToVector(cam.ang - cam.FOV / 2);
							extremidade.x += cam.pos.x;
							extremidade.y += cam.pos.y;
						}
						newVertice = intersessao(cam.pos, extremidade, setor.paredes[i].v1, setor.paredes[i].v2);
						newVertice.x += radToVector(cam.ang).x;
						newVertice.y += radToVector(cam.ang).y;
					
					}




					drawCircle(newVertice.x, newVertice.y, 5, 0, 1, 1);
					if (verticeSelection == 1) {
						novaParede.v1 = newVertice;
					}
					else {
						novaParede.v2 = newVertice;
					}
					renderParede(novaParede, cam, janela, altura, alturaCam);
				}
				else {
					drawCircle(100, 100, 10, 1, 0, 1);
					vector2D newVertice1, newVertice2;
					parede novaParede;
					extremidade1 = radToVector(cam.ang - cam.FOV / 2);
					extremidade1.x += cam.pos.x;
					extremidade1.y += cam.pos.y;
					extremidade2 = radToVector(cam.ang + cam.FOV / 2);
					extremidade2.x += cam.pos.x;
					extremidade2.y += cam.pos.y;

					newVertice1 = intersessao(cam.pos, extremidade1, setor.paredes[i].v1, setor.paredes[i].v2);
					newVertice1.x += radToVector(cam.ang).x;
					newVertice1.y += radToVector(cam.ang).y;
					newVertice2 = intersessao(cam.pos, extremidade2, setor.paredes[i].v1, setor.paredes[i].v2);
					newVertice2.x += radToVector(cam.ang).x;
					newVertice2.y += radToVector(cam.ang).y;

					if ((calcDistancia(newVertice1, setor.paredes[i].v1) > calcDistancia(setor.paredes[i].v1, setor.paredes[i].v2) ||
						calcDistancia(newVertice1, setor.paredes[i].v2) > calcDistancia(setor.paredes[i].v1, setor.paredes[i].v2)) ||
						(calcDistancia(newVertice2, setor.paredes[i].v1) > calcDistancia(setor.paredes[i].v1, setor.paredes[i].v2) ||
							calcDistancia(newVertice2, setor.paredes[i].v2) > calcDistancia(setor.paredes[i].v1, setor.paredes[i].v2))) {
					
					}

					else if (calcAngulo(verticeToVector(cam.pos, newVertice1), vetorD) < cam.FOV * 0.5 &&
						calcAngulo(verticeToVector(cam.pos, newVertice2), vetorD) < cam.FOV * 0.5 ) {

						drawCircle(newVertice1.x, newVertice1.y, 5, 1, 1, 1);
						drawCircle(newVertice2.x, newVertice2.y, 5, 1, 1, 1);

						novaParede.v1 = newVertice1;
						novaParede.v2 = newVertice2;

						renderParede(novaParede, cam, janela, altura, alturaCam);
					}

				}
		}
	}
}
void render2Dmap(sector setor) {
	int i;
	for (i = 0; i < 4; i++) {
		drawCircle(setor.paredes[i].v1.x, setor.paredes[i].v1.y, 5, 1, 0.3, 0);
		drawCircle(setor.paredes[i].v2.x, setor.paredes[i].v2.y, 5, 1, 0.3, 0);
		drawLine(setor.paredes[i].v1.x, setor.paredes[i].v1.y, setor.paredes[i].v2.x, setor.paredes[i].v2.y, 1, 0.3, 0);
	}

}


int main(int argc, char* argv[]) {

	Janela janela;
	janela.altura = 500;
	janela.largura = 1000;
	janela.Title = L"Sua aplicação";
	MSG mensagem;
	int rodando=1, i, mapOn=0;
	char str[20];
	_int64 contador;
	double performance;
	LARGE_INTEGER timeStart, Frequency;

	//

	camera cam;
	entidade inimigo;
	Texture T_inimigo;
	vector2D vertices[8];
	parede paredes[8];
	sector sec1, sec2;

	// player config
	cam.pos.x = janela.largura / 2;
	cam.pos.y = janela.altura / 2;
	cam.ang = 0;
	cam.FOV = 3.1415 / 2.8;
	//entity config
	inimigo.pos.x = janela.largura / 2;
	inimigo.pos.y = janela.altura / 4;
	iniRect(&inimigo.rect, 0, 0, 100,200);
	// map config
	vertices[0].x = janela.largura/4; vertices[0].y = 100;
	vertices[1].x =janela.largura/2+janela.largura/4; vertices[1].y = 100;
	vertices[2].x = janela.largura / 2 + janela.largura / 4;  vertices[2].y = 400;
	vertices[3].x = janela.largura / 4; vertices[3].y = 400;

	vertices[4].x = janela.largura / 3.5; vertices[4].y = janela.altura / 4;
	vertices[5].x = (janela.largura / 3.5)+100; vertices[5].y = janela.altura / 4;
	vertices[6].x = (janela.largura / 3.5)+100; vertices[6].y = (janela.altura / 4)+100;
	vertices[7].x = janela.largura / 3.5; vertices[7].y = (janela.altura / 4) + 100;

	paredes[0].v1 = vertices[0]; paredes[0].v2 = vertices[1];
	paredes[1].v1 = vertices[1]; paredes[1].v2 = vertices[2];
	paredes[2].v1 = vertices[2]; paredes[2].v2 = vertices[3];
	paredes[3].v1 = vertices[3]; paredes[3].v2 = vertices[0];

	paredes[4].v1 = vertices[4]; paredes[4].v2 = vertices[5];
	paredes[5].v1 = vertices[5]; paredes[5].v2 = vertices[6];
	paredes[6].v1 = vertices[6]; paredes[6].v2 = vertices[7];
	paredes[7].v1 = vertices[7]; paredes[7].v2 = vertices[4];

	sec1.paredes[0] = paredes[0];
	sec1.paredes[1] = paredes[1];
	sec1.paredes[2] = paredes[2];
	sec1.paredes[3] = paredes[3];

	sec2.paredes[0] = paredes[4];
	sec2.paredes[1] = paredes[5];
	sec2.paredes[2] = paredes[6];
	sec2.paredes[3] = paredes[7];



	StartWindow(*argv,argc, &janela);

	createTexture(&inimigo.tex,"sprite.bmp");
	
	QueryPerformanceFrequency(&Frequency);
	while (rodando==1) {
		QueryPerformanceCounter(&timeStart);
		contador = timeStart.QuadPart;
		if (PeekMessage(&mensagem, NULL, 0, 0, PM_REMOVE))
		{
			if (mensagem.message == WM_QUIT)
			{
				rodando = 0;
			}
			else if (mensagem.message == WM_KEYDOWN) {
				if (mensagem.wParam == VK_RETURN) {
					if (mapOn == 0) {
						mapOn = 1;
					}
					else {
						mapOn = 0;
					}
				}
			}
			else
			{
				TranslateMessage(&mensagem);
				DispatchMessage(&mensagem);
			}
		}
		else {

			if (GetKeyState('A') & 0x80000) {
				cam.ang -= 0.05;
			}
			if (GetKeyState('D') & 0x80000) {
				cam.ang += 0.05;
			}
			if (GetKeyState('W') & 0x80000) {
				cam.pos.x = cam.pos.x+(radToVector(cam.ang).x*5);
				cam.pos.y = cam.pos.y+(radToVector(cam.ang).y*5);
			}
			if (GetKeyState('S') & 0x80000) {
				cam.pos.x = cam.pos.x + (-radToVector(cam.ang).x * 5);
				cam.pos.y = cam.pos.y + (-radToVector(cam.ang).y * 5);
			}


			BeginDrawSection();

			// render map
			renderSector(sec1, cam, janela, 300, 150);
			//renderSector(sec2, cam, janela, 500, 150);
			
			// render inimigo
		
			if (visibilidadeEntidade(inimigo, cam)) {
				drawCircle(inimigo.pos.x, inimigo.pos.y, 10, 0, 0, 1);
				renderEntidade(inimigo, cam, janela, 150);
			}


			if (mapOn == 1) {

				//render2Dmap
				render2Dmap(sec1);
				//render2Dmap(sec2);
				//render player

				drawCircle(cam.pos.x, cam.pos.y, 10, 1, 0, 0);
				drawLine(cam.pos.x, cam.pos.y, cam.pos.x + radToVector(cam.ang).x * 50, cam.pos.y + radToVector(cam.ang).y * 50, 1, 1, 0);
				//drawLine(cam.pos.x, cam.pos.y, cam.pos.x + radToVector(cam.ang + (cam.FOV / 2)).x * 1000, cam.pos.y + radToVector(cam.ang + (cam.FOV / 2)).y * 1000, 1, 1, 0);
				//drawLine(cam.pos.x, cam.pos.y, cam.pos.x + radToVector(cam.ang - (cam.FOV / 2)).x * 1000, cam.pos.y + radToVector(cam.ang - (cam.FOV / 2)).y * 1000, 1, 1, 0);
			}
			
			RefreshBuffer(&janela.hDC);
			
			
			QueryPerformanceCounter(&timeStart);
			performance = (timeStart.QuadPart - contador);
			performance * 1000000;
			performance /= Frequency.QuadPart;
			performance = 1/performance;

			sprintf_s(str,20,"FPS:%f",performance);

			SetWindowTextA(janela.hwnd, str);

		}
	}

	return 0;
}