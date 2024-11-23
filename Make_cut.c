#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

//============================================================

/* FindWord
    Фунция ищет слово в массиве
    Словом является набор символом отделенный пробелами, табуляцией, новой строкой

    char *r_txt - указатель на массив исходного текста
    unsigned int *cursor - указатель на курсор считывания
    char *flag - флажок окончания считывания

*/
unsigned int FindWord (char *r_txt, unsigned int *cursor, char *flag);


//------------------------------------------------------------

/* GetWord
    Функция копирует слово из массива
    По указателю и длине слова, выделяется память и копируется массив символов в память

    unsigned int *cursor - указатель на курсор считывания
    char *word - указатель на массив слова
    char *txt - указатель на массив исходного текста
    unsigned int leight - длина слова

*/
char* GetWord (unsigned int *cursor, char *word, char *txt, unsigned int leight);


//------------------------------------------------------------

/* chopWord
    Функция разделяет слово на мягкие переносы
    По шаблону распознаются и вставляются "\-" в слово

    char *word - указатель на массив слова
    char *patt - указатель на массив шаблона
*/
void chopWord (char *word, char *patt);


//------------------------------------------------------------

/* putIn
    Функция производит вставку символа в массив
    Проихводится смещение вправо на один элемент начиная с определенного элемента массива, в образованное место вставляется символ

    int add - указатель на первый смещаемый элемент
    char *line - указатель на массив
    char pp - вставной символ
*/
void putIn (int add, char *line);


//------------------------------------------------------------

/* detectWord
    Функция определяет шаблон слова
    Наполняет массив шаблона специальными символами, в прямом соответствии с шаблоном буквы из слова и положением буквы в слове.

    char *word - указатель на массив слова
    char *patt - указатель на массив шаблона
    char return - значение проверки на слово/не
*/
char detectWord (char *word, char *patt);


//------------------------------------------------------------

/* detectLetter
    Функция определяет шаблон буквы
    Сравнивает код буквы со списком и возвращает код шаблона этой быквы

    int Letter - буква
    int flag - метка для русской буквы
    int return - шаблон буквы
*/
int detectLetter (int Letter, int flag);


//============================================================


int main(int argc, char **argv)
{

	FILE *file_input, *file_output;

	if (argc < 2)
	{
		printf("Enter file.tex after start program");
		exit(-1);
	}
	
	struct stat file_stat; //information about readen file

	file_input = fopen (argv[1], "r");
	file_output = fopen("output.txt", "w");

	if(stat(argv[1], &file_stat))
	{
		printf("syscal stat didn't work correctly\nPlease check file\n");
		exit(-1);
	}

	if (!(file_input && file_output))
	{
		printf("File don't opened");
		exit(-1);
	}
 
	stat(argv[1], &file_stat);

	unsigned int Size_of_file = file_stat.st_size * 2;
	
	char *r_txt = (char*)calloc(Size_of_file, sizeof(char)); 

	unsigned int fread_return_value = fread(r_txt, sizeof(char), Size_of_file, file_input);

	//printf("fread_return_value %u, Size_of_file %u\n", fread_return_value, (unsigned int)file_stat.st_size);

	char flag_do = 1;
	unsigned int cursor = 0;
	unsigned int word_line = 0;

	//for (int i = 0; i < Size_of_file; ++i) printf("%i %i %c\n", i, r_txt[i], r_txt[i]);

	//printf("%s\n", r_txt);

	while (flag_do)
	{
		word_line = FindWord (r_txt ,&cursor, &flag_do); // 1 - get word from text's array

		//printf("%u\n", cursor);

		char *word = (char*)calloc(word_line * 3 - 2, sizeof(char));
		char *patt = (char*)calloc(word_line * 3 - 2, sizeof(char));

		word = GetWord (&cursor, word, r_txt, word_line);

		//for (int i = 0; i < strlen(word); ++i) printf("%i %c\n", word[i], word[i]);

		printf("%s\n", word);

		chopWord(word, patt);

		//for (int i = 0; i < strlen(word); ++i) printf("%i %c\n", patt[i], patt[i]);

		printf("%s\n", patt);

		fprintf(file_output, "%s", word);

		cursor++;

		free(word);
		free(patt);

	}

	free(r_txt);
	fclose(file_output);
	fclose(file_input);

	return 0;
}

//============================================================

unsigned int FindWord (char *r_txt, unsigned int *cursor, char *flag)
{
	unsigned int pos_f = *cursor;
	unsigned int pos_s = 0;

	//printf("First: %u %u %u %i\n", pos_f, pos_s, *cursor, *flag);

	while (r_txt[*cursor] != '\0' && r_txt[*cursor] != '\n' && r_txt[*cursor] != '\t' && r_txt[*cursor] != ' ' && r_txt[*cursor] != EOF)
	{
		//printf("a\n");
		(*cursor)++;
	}

	//printf("Second: %u %u %u %i\n", pos_f, pos_s, *cursor, *flag);

	pos_s = *cursor;

	//printf("Trird: %u %u %u %i\n", pos_f, pos_s, *cursor, *flag);

	if (r_txt[pos_s] == '\0')
	{
		*flag = 0;
	}

	//printf("Fourth: %u %u %u %i\n", pos_f, pos_s, *cursor, *flag);

	return pos_s - pos_f;
}

//============================================================

char* GetWord (unsigned int *cursor, char *word, char* txt, unsigned int leight)
{
	//printf("I get: %u %u\n", *cursor, leight);
	for (int i = 0; i < leight + 1; ++i)
	{
		word[i] = txt[i + *cursor - leight];
	}

	return word;
}

//============================================================

char detectWord (char *word, char *patt)
{
	//for (int i = 0; i<1000; i++) printf ("%c\n", word[i]);
    int result,
        flag = 0;

    unsigned int leight = strlen(word);

	for (int i = 0; i < leight; ++i)
	{
        //printf ("before %d\t\t", flag);
        if (flag < 0)
            flag++;

        if (flag > 0)
            flag--;
        //printf ("after %d\t\t", flag);

		result = detectLetter(word[i], flag);
        //printf ("%d %c ", result, result);

		if (result == 'X')
		{
            flag = 2;
            patt[i] = 'r';
        }
        else if (result == 'Y')
		{
            flag = -2;
            patt[i] = 'r';
        }
        else patt[i] = result;
        //printf ("which? %d\t\t", flag);
	}

	//printf("%s\n", patt);

	int cBig = 0,
		stBf = 1,
		stAf = 0,
		fWrd = 1;
        //fNum = 1;
	for (int i = 0; i < leight; i++)
	{
        if (patt[i] == 'B' || patt[i] == 'A')
            cBig++;

        /*
        if (patt[i] != 'n')
            fNum = 0;
		*/

        if (patt[i] == 'B' || patt[i] == 'b' || patt[i] =='A' || patt[i] == 'a' || patt[i] == '-' || patt[i] == 'r' || patt[i] == 'd' || patt[i] == 'D' || patt[i] == 'q' || patt[i] == 'Q')
        {
        	if (stBf == 1)
        		stBf = 0;
        	else if (stAf == 1)
        		fWrd = 0;
        }
        else if (stBf == 0) stAf = 1;
    }

	//printf("%s\n", patt);


    if (cBig > 1)
        return 'A';

    /*
    if (fNum == 1)
        return 'N';
	*/
    //printf("%d\n", fWrd);
    if (fWrd == 1)
    	return 'W';

    return 'F';
}

//============================================================

void chopWord (char *word, char *patt)
{
	char type = detectWord(word, patt);
	//printf ("__%c__", type);
	//for (int i = 0; i<1000; i++) putchar(patt[i]);
    if (type != 'W')
        return;

	//printf("%s\n", patt);

    unsigned int leight = strlen(word);

    int cAa = 0,
        cRr = 0;
    for (int i = 0; i < leight; i++)
	{
        if (patt[i] == 'r')
            cRr++;
        if (patt[i] == 'A' || patt[i] == 'a')
            cAa++;
    }

    int cAan = 0,
        cRrn = 0,
        posA = -1,
        posB = -1,
        cCut = -1;

    for (int i = 0; patt[i] != '\0'; i++)
    {
    	//printf("eto %c\n", patt[i]);
        if (patt[i] == 'r')
            cRrn++;

    	if (patt[i] == 'D' || patt[i] == 'd')
    		{
        		//printf ("CUT -\n");
				//printf ("Cut (reason 1)\n");

                if (cRrn == cRr || cRrn == cRr - 1)
                    return;

                cCut = cCut + 2;
                posA = i + cCut;
                //printf("%i\n", posA);
                putIn(posA, word);
                posA = posB = -1;

    			continue;
    		}

        if (patt[i] == 'q' || patt[i] == 'Q')
            {
                //printf ("CUT -\n");
                //printf ("Cut (reason 3)\n");
                if (cRrn == cRr || cRrn == 1)
                    continue;
                cCut = cCut + 2;
                posA = i - 2 + cCut;
                //printf("%i\n", posA);
                putIn(posA, word);
                posA = posB = -1;
                continue;
            }

    	if (patt[i] == '-')
    		{
        		//printf ("CUT -\n");
				//printf ("Cut (reason 3)\n");
    			posA = posB = -1;
    			continue;
    		}

        if (patt[i] == 'a' || patt[i] == 'A')
        {
        	//printf ("CUT A\n");
            if (posA == -1)
            {
                posA = i;
            	posB = i;
        		//printf ("CUTCH A = %d B = %d\n", posA, posB);
    			continue;
            }

            if (posA != -1)
            {
                posB = i;
        		//printf ("CUTCH A = %d B = %d\n", posA, posB);
                if (((posB - posA) == 2) || ((posB - posA) == 4))
                {
                    if ((cRrn == 2 || cRrn == 3) && posA == 1)
                    {
                        posA = i;
                        posB = i;
                        continue;
                    }

                    if (cRrn == cRr && ((posB - posA) == 2))
                    {
                        continue;
                    }
					//printf ("Cut (reason 2)\n");
                	cCut = cCut + 2;
                	posA = posA + cCut;
                	//printf("%i\n", posA);
                	putIn(posA, word);
                    /*
                    for (int a = 0; a < 1000; ++a)
                    {
                        if (word[a] != 0)
                            printf("%i_", word[a]);
                    }
                    printf("\n");
                    */
                	posA = posB;
    				continue;
                }

                if ((posB - posA) > 4)
                {
					//printf ("Cut (reason 1)\n");
                	cCut = cCut + 2;
                	posA = posA + cCut + 2;
                	//printf("%i\n", posA);
                	putIn(posA, word);
                    /*
                    for (int a = 0; a < 1000; ++a)
                    {
                        if (word[a] != 0)
                            printf("%i_", word[a]);
                    }
                    printf("\n");
                    */
                	posA = posB;
    				continue;
                }
            }
            cAan++;
            if (cAan == cAa)
            	return;

        }
    }
}

//============================================================

void putIn (int add, char *line)
{
    unsigned int leight = strlen(line);
    
    for (int i = leight; i >= add; --i)
        line[i + 2] = line[i];

    line[add] = 92;
    line[add + 1] = '-';
}

//============================================================

int detectLetter (int Letter, int flag)
{

    //printf ("\n%d %d chek ", Letter, flag);
    if (Letter == -48)
    {
        //printf ("YYYY\n");
            return 'X';
    }
    if (Letter == -47)
    {
        //printf ("EEE)))\n");
            return 'Y';
    }


    if (flag == 0)
	switch (Letter)
	{
	case 0: return 0;

	case '-':
	return '-';

	case '\n':
    case '\t':
    case '!':
    case '\"':
    case '\'':
    case '(':
    case ')':
    case '.':
    case ',':
	case ':':
    case ';':
    case '?':
    case '[':
    case ']':
    case '{':
    case '}':
    case '`':
    case '~':
	return '!';

	case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
	return 'n';

	default: return '?';
	}

	//if (flag == 1) printf ("IM HERE\n");
	if (flag == 1)
	switch (Letter)
	{
	case -111://Б
    case -110://В
    case -109://Г
    case -108://Д
    case -106://Ж
    case -105://З
    case -103://Й
    case -102://К
    case -101://Л
    case -100://М
	case -99://Н
    case -97://П
    case -96://Р
    case -95://С
    case -94://Т
    case -92://Ф
    case -91://Х
    case -89://Ч
    case -88://Ш
	case -87://Щ
	return 'B';

	case -79://б
    case -78://в
    case -77://г
    case -76://д
    case -74://ж
    case -73://з
    case -71://й
    case -70://к
    case -69://л
    case -68://м
	case -67://н
    case -65://п
	return 'b';

	case -112://А
    case -107://Е
    case -127://Ё
    case -104://И
    case -98://О
    case -93://У
    case -85://Ы
    case -83://Э
    case -82://Ю
    case -81://Я
	return 'A';

	case -80://а
    case -75://е
    case -72://и
    case -66://о
	return 'a';

	case -86://Ъ
    case -84://Ь
	return 'D';

    case -90://Ц
    return 'Q';

	default: return 'x';

	}

	if (flag == -1)
	switch (Letter)
	{

	case -128://р
    case -127://с
    case -126://т
    case -124://ф
    case -123://х
    case -121://ч
    case -120://щ
	case -119://ш
	return 'b';

	case -111://у
    case -125://ё
    case -117://ы
    case -115://э
    case -114://ю
    case -113://я
	return 'a';

	case -118://ъ
    case -116://ь
	return 'd';

    case -122://ц
    return 'q';

	default: return 'y';

	}
	return -1;
}
