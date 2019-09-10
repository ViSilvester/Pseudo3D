# include "Janela.h"
# include "input.h"
# define _CRT_SECURE_NO_WARNINGS

typedef struct { float x; float y;}vector2D;
typedef struct { vector2D v1; vector2D v2; } parede;
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
	return sqrt(pos1.x * pos1.x + pos2.y * pos2.y);
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
	
	ent.rect.W *= 1000/(distancia+1);
	ent.rect.H *= 1000/(distancia+1);
	ent.rect.X = (ent.rect.W/2)+janela.largura - ((angulo / cam.FOV) * janela.largura);
	ent.rect.Y = (janela.altura / 2) - (ent.rect.H / 2);

	drawTexture(ent.tex,&ent.rect,NULL);
	
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

	cam.pos.x = janela.largura / 2;
	cam.pos.y = janela.altura / 2;
	cam.ang = 0;
	cam.FOV = 3.1415 / 3;

	inimigo.pos.x = janela.largura / 2;
	inimigo.pos.y = janela.altura / 4;
	iniRect(&inimigo.rect, 0, 0, 100,200);


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
				cam.pos.y-= 5;
			}
			if (GetKeyState('S') & 0x80000) {
				cam.pos.y += 5;
			}


			BeginDrawSection();

			//render player
			drawCircle(cam.pos.x, cam.pos.y, 10, 1, 0, 0);
			drawLine(cam.pos.x, cam.pos.y, cam.pos.x + radToVector(cam.ang).x * 50, cam.pos.y + radToVector(cam.ang).y * 50, 1, 1, 0);

			if (visibilidadeEntidade(inimigo, cam)) {
				drawCircle(inimigo.pos.x, inimigo.pos.y, 10, 0, 0, 1);
				renderEntidade(inimigo, cam, janela);
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