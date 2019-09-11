# include "Janela.h"
# include "input.h"
# define _CRT_SECURE_NO_WARNINGS

typedef struct { float x; float y;}vector2D;
typedef struct { vector2D v1; vector2D v2; } parede;
typedef struct { parede p1; parede p2; parede p3; parede p4; } sector;
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
vector2D radToVector(float ang) {
	vector2D vetor;
	vetor.x = cos(ang);
	vetor.y= sin(ang);
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
void renderEntidade(entidade ent, camera cam, Janela janela) {

	vector2D vetor, extremidade;
	float distancia, angulo;

	vetor.x = ent.pos.x - cam.pos.x;
	vetor.y = ent.pos.y -cam.pos.y;
	extremidade = radToVector(cam.ang+ (cam.FOV/2));
	distancia = calcDistancia(ent.pos, cam.pos);
	angulo = calcAngulo(vetor, extremidade);
	
	ent.rect.W *= 100/distancia;
	ent.rect.H *= 100/distancia;
	ent.rect.X = (janela.largura - ((angulo / cam.FOV) * janela.largura)) - (ent.rect.W / 2);
	ent.rect.Y = (janela.altura / 2) - (ent.rect.H / 2);

	drawTexture(ent.tex,&ent.rect,NULL);
	
}
void renderVertice(vector2D vertice, camera cam, Janela janela) {
	float angulo, altura = 100, distancia;
	vector2D vetor, extremidade, ponto1,ponto2;
	vetor.x = vertice.x - cam.pos.x; vetor.y = vertice.y - cam.pos.y;
	extremidade = radToVector(cam.ang + (cam.FOV / 2));
	angulo = calcAngulo(extremidade, vetor);
	ponto1.x = ponto2.x = janela.largura - ((angulo / cam.FOV) * janela.largura);
	distancia = calcDistancia(cam.pos, vertice);
	altura *= 100 / distancia;
	ponto1.y = (janela.altura / 2) - (altura / 2);
	ponto2.y = ponto1.y + altura;
	drawLine(ponto1.x, ponto1.y, ponto2.x, ponto2.y, 1, 0.3, 0);

}
vector2D vetorUnitario(vector2D vetor){
	float modulo = sqrt((vetor.x*vetor.x) + (vetor.y*vetor.y));
	vector2D novo_vetor;

	novo_vetor.x = vetor.x / modulo;
	novo_vetor.y = vetor.y / modulo;

	return novo_vetor;
	
}
vector2D intersessao(vector2D p1, vector2D p2, vector2D p3, vector2D p4 ) {

	float A1, B1, C1;
	float A2, B2, C2;
	float A3, B3, C3;
	float X, Y;

	vector2D r;

	A1 = p1.y - p2.y;
	B1 = (p2.x - p1.x);
	C1 = (p1.x*p2.y) - (p1.y * p2.x);

	A2 = p3.y - p4.y;
	B2 = (p4.x - p3.x);
	C2 = (p3.x*p4.y) - (p3.y*p4.x);


	if (A1*p1.x + B1*p1.y + C1 == 0) {
		drawCircle(0,0,5,1,0,0);
	}
	if (A1*p2.x + B1 * p2.y + C1 == 0) {
		drawCircle(10, 0,5, 1, 0, 0);
	}

	if (A2*p3.x + B2 * p3.y + C2 == 0) {
		drawCircle(20, 0, 5, 1, 0, 0);
	}
	if (A2*p4.x + B2 * p4.y + C2 == 0) {
		drawCircle(30, 0, 5, 1, 0, 0);
	}

	if (A2 == 0) {
		drawCircle(100, 0, 5, 1, 0.5, 0);
		B2 *= -1;
	}
	else if(B2 == 0) {
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

	Y = (C2*A1 - A2 * C1) / (B1*A2 - B2*A1);
	X = (C1 - (B1 * Y)) / A1;

	r.x = X;
	r.y = Y;

	return r;
}

int main(int argc, char* argv[]) {

	Janela janela;
	janela.altura = 500;
	janela.largura = 1000;
	janela.Title = L"Sua aplicação";
	MSG mensagem;
	int rodando=1, i;
	char str[20];
	_int64 contador;
	double performance;
	LARGE_INTEGER timeStart, Frequency;

	//

	camera cam;
	entidade inimigo;
	Texture T_inimigo;
	vector2D vertices[4];
	parede paredes[4];
	sector sec1;

	// player config
	cam.pos.x = janela.largura / 2;
	cam.pos.y = janela.altura / 2;
	cam.ang = 0;
	cam.FOV = 3.1415 / 3;
	//entity config
	inimigo.pos.x = janela.largura / 2;
	inimigo.pos.y = janela.altura / 4;
	iniRect(&inimigo.rect, 0, 0, 100,200);
	// map config
	vertices[0].x = vertices[0].y = 20;
	vertices[1].x = 900; vertices[1].y = 20;
	vertices[2].x = 900;  vertices[2].y = 400;
	vertices[3].x = 20; vertices[3].y = 400;

	paredes[0].v1 = vertices[0]; paredes[0].v2 = vertices[1];
	paredes[1].v1 = vertices[1]; paredes[1].v2 = vertices[2];
	paredes[2].v1 = vertices[2]; paredes[2].v2 = vertices[3];
	paredes[3].v1 = vertices[3]; paredes[3].v2 = vertices[0];

	sec1.p1 = paredes[0];
	sec1.p2 = paredes[1];
	sec1.p3 = paredes[2];
	sec1.p4 = paredes[3];


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
			else
			{
				TranslateMessage(&mensagem);
				DispatchMessage(&mensagem);
			}
		}
		else {

			if (GetKeyState('A') & 0x80000) {
				cam.ang -= 0.1;
			}
			if (GetKeyState('D') & 0x80000) {
				cam.ang += 0.1;
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

			for (i = 0; i < 4; i++) {
				vector2D vetor;
				vetor.x = vertices[i].x - cam.pos.x;
				vetor.y = vertices[i].y - cam.pos.y;
				drawCircle(vertices[i].x, vertices[i].y, 5, 1,0.3,0);
				if (calcAngulo(vetor, radToVector(cam.ang))< cam.FOV/2) {
					renderVertice(vertices[i], cam, janela);
				}
			}

			// render inimigo
		
			if (visibilidadeEntidade(inimigo, cam)) {
				drawCircle(inimigo.pos.x, inimigo.pos.y, 10, 0, 0, 1);
				renderEntidade(inimigo, cam, janela);
			}

			drawCircle(cam.pos.x, cam.pos.y, 10, 1, 0, 0);
			drawLine(cam.pos.x, cam.pos.y, cam.pos.x + radToVector(cam.ang).x * 50, cam.pos.y + radToVector(cam.ang).y * 50, 1, 1, 0);


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