
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

char **lista_cuvinte(char **lista)
//vector de cuvinte care contine lista de cuvinte castigatoare
{
  lista = (char **) malloc (27 * sizeof(char *));
  lista[0] = "ARICI";
  lista[1] = "ATENT";
  lista[2] = "BAIAT";
  lista[3] = "CEATA";
  lista[4] = "DEBUT";
  lista[5] = "PESTE";
  lista[6] = "FIXAT";
  lista[7] = "HAMAC";
  lista[8] = "HARTA";
  lista[9] = "JALON";
  lista[10] = "JUCAM";
  lista[11] = "LACAT";
  lista[12] = "MAGIE";
  lista[13] = "NUFAR";
  lista[14] = "OASTE";
  lista[15] = "PERUS";
  lista[16] = "RIGLE";
  lista[17] = "ROMAN";
  lista[18] = "SANIE";
  lista[19] = "SCRIS";
  lista[20] = "SONDA";
  lista[21] = "TEXTE";
  lista[22] = "TIPAR";
  lista[23] = "TITAN";
  lista[24] = "ZEBRA";
  lista[25] = "VAPOR";
  lista[26] = "VATRA";
  return lista;
}

int generare_indice_cuvant(int i)
/* generez un numar din intervalul [0, 26] care reprezinta inidcele
   cuvantului castigator selectat la inceperea unui joc nou */
{
  i = 0;
  srand((unsigned)time(NULL));
  i = rand() % 26;
  return i;
}

int litera(char c) 
{
  if((c < 'a' || c > 'z') && (c < 'A' || c > 'Z'))
    return 0;
  return 1;
}

int cautare_litera(char litera, char *cuvant_corect, int *poz)
//caut litera in cuvantul corect si retin pozitia pe care se gaseste
{
  int i;
  for(i = 0; i < 5; i++)
    if(litera == cuvant_corect[i])
    {
      *poz = i;
      return 1;
    }
  return 0;
}

int verif_cuvant(char *cuvant)
//verific daca toate literele din cuvant sunt colorate cu verde
{
  int i;
  for(i = 0; i < 5; i++)
    if(cuvant[i] != 'v')
      return 0;
  return 1;
}
WINDOW *colorare_litere(WINDOW *win, int nr_linie, int p, int x, char cuvant)
//colorez literele din cuvant
//p reprezinta perechea de culori care se foloseste pentru coloarea literei
{
  mvwprintw(win, nr_linie - 1, x - 1, "---");
  mvwprintw(win, nr_linie, x - 1, "|");
  wattron(win, COLOR_PAIR(p));
  mvwaddch(win, nr_linie, x, cuvant);
  wattroff(win, COLOR_PAIR(p));
  mvwprintw(win, nr_linie, x + 1, "|");
  mvwprintw(win, nr_linie + 1, x - 1, "---");
  return win;
}

void ecran_joc()
{
  int height = 20, width = 45, x_start, y_start, x_max, y_max;
  getmaxyx(stdscr, y_max, x_max);
  x_start = (x_max - width)/2;
  y_start = (y_max - height)/2;
  //determin coordonatele pentru a pozitiona fereastra la mijloc
  WINDOW *win = newwin(height, width, y_start, x_start);
  refresh();
  box(win, 0, 0);
  getmaxyx(win, y_max, x_max);
  mvwprintw(win, 0, (x_max - 6) / 2, "WORDLE");
  refresh();
  mvwprintw(win, 2, (x_max - 13) / 2, "Enter a word:");
  wmove(win, 4, x_max / 2);
  refresh();
  wrefresh(win);
  //construiesc fereastra in care apare titlul si mesajul de la inceput

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_BLACK);
  init_pair(2, COLOR_BLACK, COLOR_WHITE);
  init_pair(3, COLOR_BLACK, COLOR_GREEN);
  init_pair(4, COLOR_BLACK, COLOR_YELLOW);
  init_pair(5, COLOR_WHITE, COLOR_BLACK);
  bkgd(COLOR_PAIR(1));
  refresh();
  wbkgd(win, COLOR_PAIR(2));
  wrefresh(win);
  //am initializat perechile de culori si am colorat fundalul si ecranul jocului
  char **lista = NULL;
  lista = lista_cuvinte(lista);
  //initializez lista
  int i, j, indice = 0;
  char cuvant_corect[6];
  strcpy(cuvant_corect, lista[generare_indice_cuvant(indice)]);
  //generez cuvantul csatigator aleatoriu
  keypad(win, true);
  int nr_incercari = 0, nr_litere, nr_linie = 4, c, x = (x_max - 12) / 2;
  char cuvant[6];
  int taste = 1;
  /* variabila taste va fi folosita pentru a nu permite utilizatorului sa
     introduca sau sa stearga litere dupa castigarea sau pierderea jocului */
  while(nr_incercari < 6)
  /* plec cu nr_incercari de la 0, asa ca acestea se vor termina cand se ajunge
     la 5 */
  {
    nr_litere = 0;
    int ok = 1;
    /* ok se face 0 cand s-au introdus 5 litere si s-a apasat enter
       pentru a nu putea apasa alte taste dupa apasarea enter-ului */
    while (ok)
     //cat timp nu s-au introdus 5 litere sau nu s-a apasat enter
    {
      c = wgetch(win);
      if (litera(c) && nr_litere < 5 && taste)
       //utilizatorul poate introduce maxim 5 litere la o incercare
      {
        for(i = (x_max - 16) / 2; i < (x_max - 16) / 2 + 16; i++)
          mvwprintw(win, 18, i, " ");
        if (c >= 'a' && c <= 'z')
          c = toupper(c);
        //transform c in litera mare
        cuvant[nr_litere++] = c;
        mvwprintw(win, nr_linie - 1, x - 1, "---");
        mvwprintw(win, nr_linie, x - 1, "|");
        mvwaddch(win, nr_linie, x, c);
        mvwprintw(win, nr_linie, x + 1, "|");
        mvwprintw(win, nr_linie + 1, x - 1, "---");
        //construiesc casuta in care se afla litera si afisez
        x += 3;
        /* x se incrementeaza cu 3 pnetru a construi o noua casuta
           fara a se suprapune cu casuta precedenta */
      }

      else if (c == KEY_BACKSPACE && taste)
      {
        if(x > (x_max - 12) / 2)
          /* nu se mai poate folosi tasta backspace dupa stergerea
            tuturor literelor din cuvant */
        {
          x -= 3;
          if(nr_linie == 4)
            mvwprintw(win, nr_linie - 1, x - 1, "   ");
          /* sterg partea de sus a casutei doar pentru cuvintele
             din prima linie pentru a pastra casutele de pe
             liniile de mai sus */
          mvwprintw(win, nr_linie, x - 1, " ");
          mvwprintw(win, nr_linie, x, " ");
          mvwprintw(win, nr_linie, x + 1, " ");
          mvwprintw(win, nr_linie + 1, x - 1, "   ");
          wmove(win, nr_linie + 1, x);
        }
        if (nr_litere != 0)
          nr_litere--;
        //se scade numarul de litere din cuvant
      }
      else if (c == '\n')
      {
        if(nr_litere < 5)
          /* daca se pasa enter cand sunt mai putin de 5 litere
             cuvantul nu este acceptat */
        {
          mvwprintw(win, 18, (x_max - 16) / 2, "Enter 5 letters!");
          wmove(win, nr_linie + 1, x);
        }
        else
        {
          ok = 0;
          // nu se mai pot folosi alte taste pentru linia curenta
          cuvant[nr_litere] = '\0';
          //in cuvant am memorat cuvantul introdus

          char culori_litere[6], copie[6];
          /* in culori_litere memorez pe fiecare pozitie una dintre
             literele 'v', 'g', 'n', care reprezinta initiala
             culorii corespunzatoare literei */
          strcpy(copie, cuvant_corect);
          for (i = 0; i < 5; i++)
            culori_litere[i] = '_';
          /* initializez vectorul cu '_' pe fiecare pozitie
             pentru a sterge ce s-a memorat anterior in vector */

          for (i = 0; i < 5; i++)
          //colorez cu verde literele care se afla in pozitia corecta
          {
            if(cuvant[i] == cuvant_corect[i])
            {
              copie[i] = '_';
              //in copie inlocuiesc literele colorate cu '_'
              culori_litere[i] = 'v';
            }
          }

          int poz;
          for(i = 0; i < 5; i++)
          {
            if(cautare_litera(cuvant[i], copie, &poz))
              //caut literele in copie
            {
              if(culori_litere[i] != 'v')
                /* daca litera nu a fost deja colorata cu verde se va colora
                   cu galben*/
              {
                culori_litere[i] = 'g';
                copie[poz] = '_';
              }
            }
            else if(culori_litere[i] != 'v')
              /* daca litera nu se gaseste in copie si nu a fost colorata
                 cu verde aceasta nu apare in cuvant*/

            {
              culori_litere[i] = 'n';
            }
          }
          culori_litere[5] = '\0';

          x = (x_max - 12) / 2;
          //folosesc vectorul construit mai sus pentru a colora literele
          for (i = 0; i < 5; i++)
            if (culori_litere[i] == 'v')
            {
              win = colorare_litere(win, nr_linie, 3, x, cuvant[i]);
              x += 3;
            }
            else if (culori_litere[i] == 'g')
            {
              win = colorare_litere(win, nr_linie, 4, x, cuvant[i]);
              x += 3;
            }
            else if (culori_litere[i] == 'n')
            {
              win = colorare_litere(win, nr_linie, 5, x, cuvant[i]);
              x += 3;
            }

          if (verif_cuvant(culori_litere))
          //daca toate literele sunt verzi jucatorul castiga
          {
            mvwprintw(win, 17, (x_max - 7) / 2, "YOU WIN!");
            mvwprintw(win, 18, (x_max - 42) / 2, "Press enter to start new game or x to exit");
            taste = 0;
            //nu se mai pot sterge sau adauga caractere
            while((c = wgetch(win)))
            {
              if (c == '\n')
              {
                for (i = 3; i <= 18; i++)
                  for (j = (x_max - 42) / 2; j < (x_max - 42) / 2 + 42; j++)
                    mvwprintw(win, i, j, " ");
                //sterg tot in afara de titlu si mesajul de la inceput
                nr_incercari = 0;
                ok = 0;
                taste = 1;
                strcpy(cuvant_corect, lista[generare_indice_cuvant(indice)]);
                nr_linie = 4;
                x = (x_max - 12) / 2;
                wmove(win, nr_linie + 1, x);
                break;
              }
              else if (c == 'x')
                endwin();
            }
          }
          else if(nr_incercari == 5)
          {
            mvwprintw(win, 16, (x_max - 9) / 2, "YOU LOSE!");
            mvwprintw(win, 17, (x_max - 21) / 2, "The answer was: %s", cuvant_corect);
            mvwprintw(win, 18, (x_max - 42) / 2, "Press enter to start new game or x to exit");
            taste = 0;
            while((c = wgetch(win)))
            {
              if (c == '\n')
              {
                for (i = 3; i <= 18; i++)
                  for (j = (x_max - 42) / 2; j < (x_max - 42) / 2 + 42; j++)
                    mvwprintw(win, i, j, " ");
                nr_incercari = 0;
                ok = 0;
                taste = 1;
                strcpy(cuvant_corect, lista[generare_indice_cuvant(indice)]);
                nr_linie = 4;
                x = (x_max - 12) / 2;
                wmove(win, nr_linie, x);
                break;
              }
              else if (c == 'x')
                endwin();
            }
          }
          else
            //se trece la urmatoarea incercare
          {
            nr_linie += 2;
            nr_incercari++;
            x = (x_max - 12) / 2;
            wmove(win, nr_linie, x);
          }
        }
      }

      else if(c == ':')
      {
        for(i = 2; i <= 18; i++)
          for(j = (x_max - 42) / 2; j < (x_max - 42) / 2 + 42; j++)
            mvwprintw(win, i, j, " ");
        mvwprintw(win, 3, (x_max - 33) / 2, "Press up arrow for first option");
        mvwprintw(win, 4, (x_max - 32) / 2, "and down arrow for second option");
        mvwprintw(win, 5, (x_max - 16) / 2, "Then press enter");
        mvwprintw(win, 8, (x_max - 16) / 2, "START NEW GAME");
        mvwprintw(win, 10, (x_max - 6) / 2, "EXIT");
        //sterg ecranul si construiesc meniul de control
        int b = -1;
        /* b va fi -1 la inceput pentru a nu permite apasarea enter
           decat dupa selectarea unei optiuni */
        while((c = wgetch(win)))
        {
          if(c == KEY_UP)
          {
            wattron(win, A_STANDOUT);
            mvwprintw(win, 8, (x_max - 16) / 2, "START NEW GAME");
            wattroff(win, A_STANDOUT);
            mvwprintw(win, 10, (x_max - 6) / 2, "EXIT");
            b = 1;
          }
          else if(c == KEY_DOWN)
          {
            mvwprintw(win, 8, (x_max - 16) / 2, "START NEW GAME");
            wattron(win, A_STANDOUT);
            mvwprintw(win, 10, (x_max - 6) / 2, "EXIT");
            wattroff(win, A_STANDOUT);
            b = 0;
          }
          /* b se schimba dupa selectarea unei optiuni si in functie
             de valoarea lui la apasarea enter se iese din joc sau
             se incepe alt joc */
          else if(c == '\n' && b == 1)
          {
            for(i = 3; i <= 10; i++)
              for(j = (x_max - 33) / 2; j < (x_max - 33) / 2 + 33; j++)
                mvwprintw(win, i, j, " ");
            mvwprintw(win, 2, (x_max - 13) / 2, "Enter a word:");
            nr_incercari = 0;
            ok = 0;
            strcpy(cuvant_corect, lista[generare_indice_cuvant(indice)]);
            nr_linie = 4;
            x = (x_max - 12) / 2;
            wmove(win, nr_linie, x);
            break;
          }
          else if(c == '\n' && b == 0)
            endwin();
          }
        }
      }
    }
  wrefresh(win);
  free(lista);
}

int main()
{
  initscr();
  cbreak();
  noecho();
  refresh();
  ecran_joc();
  getch();
  endwin();
  return 0;
}
