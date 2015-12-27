#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include<string>
#include <sstream>

#include <SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

using namespace std;

 typedef enum {LEFT,RIGHT}BUTTONPOS;//ö�ٰ�ť�ھ���ͼ�еı��
 typedef enum {a,b,c,d,e,f,g,OTHER}BUTTONFLAG;//ö�ٰ�ť
 
 //��Ļ�ߴ�
 #define SCREENWIDTH  555
 #define SCREENHEIGH  659
  #define SCREENWIDTH2  555
 #define SCREENHEIGH2  200
 
 //���������ť��λ��
int position[8][4] = {{45,170,193,147},{289,170,193,147},
								{45,321,193,147},{289,321,193,147},
								{45,460,193,147},{289,460,193,147},
								{220,530,100,30},{450,620,100,30}};

 SDL_Rect pos[9] = {{45,170,193,147},{289,170,193,147},
								{45,321,193,147},{289,321,193,147},
								{45,460,193,147},{289,460,193,147},
 {220,530,100,30},{450,620,100,30},{220,570, 100,30}};

 //SDL_Rect pos[8] = {{45,170,193,147},{289,170,193,147},
								//{45,321,193,147},{289,321,193,147},
								//{45,460,193,147},{289,460,193,147},
								//{0,0,100,30},{455,0,100,30}};

 SDL_Window *Init()
{
	SDL_Window *pScreen = NULL;
	//SDL��ʼ��
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf ("SDL initialize fail:%s\n", SDL_GetError());
		return NULL;
	}

	//�������ڣ��������ƣ�����Ļ����ʾ��ˮƽλ�ã���ֱλ�ã�������ߣ����ӣ�
	pScreen = SDL_CreateWindow("Hit the mouse",400,100 , SCREENWIDTH,SCREENHEIGH, SDL_WINDOW_SHOWN);  
    if (NULL == pScreen)//����Ƿ��ʼ���ɹ�  
        printf ("SDL_CreateWindow fail:%s\n", SDL_GetError());

    if (TTF_Init() == -1 )//��������Ƿ��ʼ���ɹ�   
		printf ("TTF_Init fail:%s\n", SDL_GetError());

	if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096)==-1)
		printf ("Mix_Init fail:%s\n", SDL_GetError());

	return pScreen;
}
  SDL_Window *Init2()
{
	SDL_Window *pScreen = NULL;
	//SDL��ʼ��
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf ("SDL initialize fail:%s\n", SDL_GetError());
		return NULL;
	}

	//�������ڣ��������ƣ�����Ļ����ʾ��ˮƽλ�ã���ֱλ�ã�������ߣ����ӣ�
	pScreen = SDL_CreateWindow("SCORE",400,300 , SCREENWIDTH2,SCREENHEIGH2, SDL_WINDOW_SHOWN);  
    if (NULL == pScreen)//����Ƿ��ʼ���ɹ�  
        printf ("SDL_CreateWindow fail:%s\n", SDL_GetError());

    if (TTF_Init() == -1 )//��������Ƿ��ʼ���ɹ�   
		printf ("TTF_Init fail:%s\n", SDL_GetError());

	return pScreen;
}

//�ͷ�
void Quit()
{
	TTF_Quit();	//�ͷ�����

	SDL_Quit();	//�˳�SDL
}

//����ͼƬ
SDL_Texture *Load_image(char *filename,SDL_Renderer *render)
{
	SDL_Surface *img = NULL;
	SDL_Texture *texture = NULL;

	img=IMG_Load(filename);//����ͼƬ��������ʱ����

	if (img == NULL)//���ͼƬ�Ƿ���سɹ�
		printf ("IMG_Load fail:%s", SDL_GetError());

	texture = SDL_CreateTextureFromSurface(render, img);//���ɹ�������

	if (texture == NULL) 
        printf("SDL_CreateTextureFromSurface fail:%s", SDL_GetError());

	//�ͷ���ʱ����
	SDL_FreeSurface(img);

	return texture;
}

//��������
SDL_Texture* RenderText(SDL_Renderer * render,string message, string fontFile, SDL_Color color, int fontSize)
{
    TTF_Font *font = nullptr;
    font = TTF_OpenFont(fontFile.c_str(), fontSize);//�������ļ����������С
    if (font == nullptr)
        throw std::runtime_error("Failed to load font: " + fontFile + TTF_GetError());
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);//����������ɫ
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surf);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}

//������Ⱦ
void Copy_render(SDL_Renderer * render, SDL_Texture *texture, int x = 0, int y = 0, int w = SCREENWIDTH, int h = SCREENHEIGH)
{
	double angle = 0.0;//��ת��
	int p_x = 0;//Բ��
	int p_y = 0;

	SDL_Rect rect = {x, y, w, h};//ԭ����
	SDL_Point center = {p_x, p_y};

	//��ԭ���ε�Բ��Ϊ���ģ�����
	SDL_RenderCopyEx(render, texture, NULL, &rect, angle, &center, SDL_FLIP_NONE);
}



 SDL_Texture *gpBackground = NULL;//����
 SDL_Texture *gpFM = NULL;
 SDL_Texture *gpButton = NULL;
 SDL_Texture *gpStart=NULL;
 SDL_Texture *gpQuit=NULL;
 SDL_Texture *gpTitle=NULL;
 SDL_Texture *gpBumb=NULL;
 SDL_Texture *gpRad=NULL;
 SDL_Texture *word = NULL;
 SDL_Texture *word2 = NULL;
 Mix_Music *Music1 = NULL;
 Mix_Music *Music2 = NULL;



 SDL_Event myEvent;//�¼�
 
 int isOnButton(int aX,int aY,int aIndex);

 int main(int argc,char *argv[])
 {
     int quit = 1,k;
     int x,y;
	 time_t t;
     BUTTONFLAG selected = OTHER;//��ťѡ�б�־
     int current=-1;
	 int i = 0;

	 stringstream stream;
	 string str;
	 /*stream << i;
	 stream >> str; */

	 //SDL��ʼ��
	SDL_Window *gpScreen = Init();


	//��ʼ������
	SDL_Renderer *render = SDL_CreateRenderer(gpScreen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	//���屳��ɫ
	SDL_SetRenderDrawColor(render,255,255,255,255);

	Music1=Mix_LoadMUS("death_box.ogg");
	Music2=Mix_LoadMUS("mis.wav");

     //�������ͼƬ
     gpBackground = Load_image("bground_01.jpg",render);
	 gpTitle = Load_image("1_03.png",render);
	 gpFM = Load_image("fengmian.png",render);

	 SDL_RenderClear(render);
	 Copy_render(render, gpFM, 0, 0, SCREENWIDTH,SCREENHEIGH);//��������
	 Copy_render(render, gpTitle, 120, 150, 320,200);
	 
	 gpButton = Load_image("hit.png",render);

     gpStart = Load_image("start.png",render);
	 Copy_render(render, gpStart,position[6][0],position[6][1],position[6][2],position[6][3]);//��������

	 gpQuit = Load_image("quit_03.png",render);
	 Copy_render(render, gpQuit, 220, 570, 100,30);//��������
	 SDL_RenderPresent(render);

	 gpBumb=Load_image("zhong.png",render);

	 gpRad=Load_image("00_03.png",render);

     if (!gpBackground||!gpButton)
	 {
         exit(0);
     }

	 while(quit)
	 {
		 while(SDL_PollEvent(&myEvent))
		 {
			 switch (myEvent.type)
			 {
			 case SDL_MOUSEBUTTONDOWN:
				 switch(myEvent.button.button)
				 {
				 case SDL_BUTTON_LEFT:
					 x=myEvent.button.x;
					 y=myEvent.button.y;
					  if (isOnButton(x,y,6)==1)
                     {
						 selected = g;
                         quit=0;
                     }

					  if (isOnButton(x,y,8)==1)
                     {
                         Quit();
                     }
				 }
			 }
		 }
	 }
	
     //�¼�����
     while (!quit)
     {
		 /*stream << i;
	     stream >> str; */

		srand((unsigned)time(&t));
		k=rand()%6;
		if(k<6)
		{ 
			SDL_RenderClear(render);
			Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
			Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
			Copy_render(render, gpRad, position[k][0], position[k][1], position[k][2],position[k][3]);//��������

			//SDL_Color color = {0, 0, 0};//��ɫ
			///*word = RenderText(render,str, "simfang.ttf", color, 64);*/
			//word = RenderText(render,"SCORE:"+i, "simfang.ttf", color, 64);
			//Copy_render(render, word, 0,0,100,30);
			SDL_RenderPresent(render);
		}
		
         while (SDL_PollEvent(&myEvent))
         {

			 /*stream << i;
			 stream >> str; */

             switch (myEvent.type)//����¼�����
             {
             case SDL_QUIT:
                 quit = 1;
                 break;
             case SDL_MOUSEBUTTONDOWN://��갴��
                 switch(myEvent.button.button)
                 {
                 case SDL_BUTTON_LEFT:
                     x = myEvent.button.x;//�õ���ǰ��������
                     y = myEvent.button.y;                   

                     //�ж�����Ƿ�����ע�ᰴť��
                     if (isOnButton(x,y,0)==1)
                     {
                         selected=a;    
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[0][0], position[0][1], position[0][2],position[0][3]);//��������

						 SDL_RenderPresent(render);
                     }
                     else if (isOnButton(x,y,1)==1)//�ж�����Ƿ�����ע�ᰴť��
                     {
                         selected=b;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[1][0], position[1][1], position[1][2],position[1][3]);//��������

						 SDL_RenderPresent(render);
                     }
					 else if (isOnButton(x,y,2)==1)//�ж�����Ƿ�����ע�ᰴť��
                     {
                         selected=c;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[2][0], position[2][1], position[2][2],position[2][3]);//��������

						 SDL_RenderPresent(render);
                     }  
					 else if (isOnButton(x,y,3)==1)//�ж�����Ƿ�����ע�ᰴť��
                     {
                         selected=d;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[3][0], position[3][1], position[3][2],position[3][3]);//��������

						 SDL_RenderPresent(render);
                     }  
					 else if (isOnButton(x,y,4)==1)//�ж�����Ƿ�����ע�ᰴť��
                     {
                         selected=e;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[4][0], position[4][1], position[4][2],position[4][3]);//��������

						 SDL_RenderPresent(render);
                     }  
					 else if (isOnButton(x,y,5)==1)//�ж�����Ƿ�����ע�ᰴť��
                     {
                         selected=f;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpBumb, position[5][0], position[5][1], position[5][2],position[5][3]);//��������

						 SDL_RenderPresent(render);
                     }
					 else if (isOnButton(x,y,7)==1)//�ж�����Ƿ�����ע�ᰴť��
					 {
                        quit=1;
						
                     }  
                     break;                
                 }
                 break;
             case SDL_MOUSEBUTTONUP:
                 switch(myEvent.button.button)
                 {
                 case SDL_BUTTON_LEFT:
                     x = myEvent.button.x;//�õ���ǰ��������
                     y = myEvent.button.y;                    

                     //�ж�����Ƿ�����ע�ᰴť��
                     if (selected==a&&k==0)
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[0][0], position[0][1], position[0][2],position[0][3]);//��������

						 SDL_RenderPresent(render);

						 if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);
						 
						 printf("%s%d","����Ϊ��",i);
                     }
                     else if (selected==b&&k==1)//�ж�����Ƿ�����ע�ᰴť��
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[1][0], position[1][1], position[1][2],position[1][3]);//��������

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","����Ϊ��",i);
                     }
					  else if (selected==c&&k==2)//�ж�����Ƿ�����ע�ᰴť��
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[2][0], position[2][1], position[2][2],position[2][3]);//��������

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","����Ϊ��",i);
                     } 
					  else if (selected==d&&k==3)//�ж�����Ƿ�����ע�ᰴť��
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[3][0], position[3][1], position[3][2],position[3][3]);//��������

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","����Ϊ��",i);
                     } 
					   else if (selected==e&&k==4)//�ж�����Ƿ�����ע�ᰴť��
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[4][0], position[4][1], position[4][2],position[4][3]);//��������

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","����Ϊ��",i);
                     } 
					   else if (selected==f&&k==5)//�ж�����Ƿ�����ע�ᰴť��
                     {
						 i++;
						 SDL_RenderClear(render);
						 Copy_render(render, gpBackground, 0, 0, SCREENWIDTH,SCREENHEIGH);
						 Copy_render(render, gpQuit, position[7][0], position[7][1], position[7][2],position[7][3]);
						 Copy_render(render, gpButton, position[5][0], position[5][1], position[5][2],position[5][3]);//��������

						 SDL_RenderPresent(render);

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music1,0);

						 printf("%s%d","����Ϊ��",i);
                     } 
					   else if (selected==g){}
					   else
					   {
						 selected = OTHER;

						  if(Mix_PlayingMusic()==1)
							 Mix_HaltMusic();
						 else
							 Mix_PlayMusic(Music2,0);

						 printf("%s%d","����Ϊ��",i);	
					   } break;
                 }
                 break;
             }            
         }
     }
	 stream << i;
	     stream >> str; 
	 SDL_Window *gpScreen2 = Init2();

	 while(quit)
	{
		 //��ʼ������
	SDL_Renderer *render2 = SDL_CreateRenderer(gpScreen2, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	 SDL_SetRenderDrawColor(render2,183,255,159,255);	//ǳ��ɫ

	 SDL_RenderClear(render2);
	 //ApplyString(render,image, word, keyboard, letter, buf, clor

			SDL_Color color = {0, 0, 0};//��ɫ
			word = RenderText(render2,"Your final score is :  "+str, "simfang.ttf", color, 32);
			word2 = RenderText(render2,"press Esc to exit... ", "simfang.ttf", color, 12);
			Copy_render(render2, word, 20,55,535,50);
			Copy_render(render2, word2, 180,130,250,20);
			SDL_RenderPresent(render2);


				if (SDL_PollEvent (&myEvent))	
	{
		switch (myEvent.type)
		{
		case  SDL_QUIT:	//������뿪�¼����˳�����
			quit = 0;
			break;
		case SDL_KEYDOWN:	//����Ǽ��̰����¼����Ұ���ΪESC���˳�����
			if (myEvent.key.keysym.sym == SDLK_ESCAPE)
				quit = 0;
			break;
		}
		
	}
	 }

	 
	//SDL_FreeSurface(letter);
	SDL_DestroyTexture(word);

	SDL_DestroyTexture(gpBackground);//�ͷ��ڴ�
	SDL_DestroyTexture(gpFM);
	SDL_DestroyTexture(gpButton);
    SDL_DestroyTexture(gpStart);
	SDL_DestroyTexture(gpQuit);
	SDL_DestroyTexture(gpBumb);
	SDL_DestroyTexture(gpRad);
	SDL_DestroyTexture(gpTitle);
	Mix_FreeMusic(Music1);
	Mix_FreeMusic(Music2);

	 SDL_DestroyRenderer(render);
	 //SDL_DestroyRenderer(render2);
     SDL_DestroyWindow(gpScreen);
	 SDL_DestroyWindow(gpScreen2);

     Quit();
     system("pause"); 
     return 0; 
 
	 }
 
 /*--------------------------------------------------------------------
     ������:    isOnButton
     ��  ��:    (x,y)��굱ǰ����;
             indexҪ�жϵ��Ǹ���ť,0��ʾ��¼��ť��1��ʾע�ᰴť
     ����ֵ: �������ָ����ť�Ϸ���1�����򷵻�0
     ��  ��:    �ж�����Ƿ�����ָ����ť����
     ��  ע:
 ----------------------------------------------------------------------*/
 int isOnButton(int aX,int aY, int aIndex)
 {
     return aX>=pos[aIndex].x && aX<=(pos[aIndex].x+pos[aIndex].w)&&
         aY>=pos[aIndex].y && aY<=(pos[aIndex].y + pos[aIndex].h);
 }
 
 
 