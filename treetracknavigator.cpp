#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
// dichiarazione di un nodo
typedef struct el_2
{
   int value;
   struct el_2 *next;

} elem_2;
typedef elem_2 *nodo_l;
typedef struct el
{
   int dist;
   int max;
   nodo_l list;
   struct el *p;
   struct el *right;
   struct el *left;
} elem;
typedef elem *nodo;

nodo inserisci(nodo, int, int *);
void visita(nodo);
nodo ricerca(nodo, int);
nodo elimina(nodo, int);
nodo min(nodo);
nodo max(nodo);
nodo predecessore(nodo);
nodo successore(nodo);
nodo_l elimina_macchina(nodo, int);
void stampa_macchine(nodo);
// aggiunge direttamnete quando ho gia il puntatore al nodo
nodo_l aggiungimacchina(nodo, int);
void elimina_lista(nodo);
int trova_massimo(nodo_l);
void stampa_macchine(nodo);
int shortest_path(nodo, nodo, nodo);
nodo shortest_path_2(nodo, nodo);
nodo nodoragg(nodo);
int app(nodo, nodo, FILE *, nodo_l*);
nodo_l aggiungitappa(nodo_l, int);
void elimina_lista_2(nodo_l);
int prima_passata(nodo, nodo, nodo_l*);
nodo_l seconda_passata(nodo, nodo_l, int *);
nodo_l lista_rovesciata(nodo_l);
int main()
{
   nodo root = NULL;
   nodo t;
   char buffer[50];
   int numerostazioni;
   int k, distanza, autonomia, partenza, arrivo, pres;
   int *p = &pres;
   FILE *pf =stdin;

   if (pf != NULL)
   {
      if(fscanf(pf, "%s", buffer));
      while (!feof(pf))
      {

         if (strcmp(buffer, "aggiungi-stazione") == 0)
         {
            *p = 0; 
            if(fscanf(pf, "%d", &distanza));
            if(fscanf(pf, "%d", &numerostazioni));
            root = inserisci(root, distanza, p);
               t = ricerca(root, distanza);
               while (numerostazioni > 0)
               {
                  if(fscanf(pf, "%d", &k));
                  if(*p==0)
                     t->list = aggiungimacchina(t, k);
                  numerostazioni--;
               }
      
         }
         else if (strcmp(buffer, "aggiungi-auto") == 0)
         {
            if(fscanf(pf, "%d", &distanza));
            t = ricerca(root, distanza);
            if(fscanf(pf, "%d", &autonomia));
            if (t != NULL)
            {
               t->list = aggiungimacchina(t, autonomia);
               printf("aggiunta");
            }
            else
               printf("non aggiunta");
         }
         else if (strcmp(buffer, "demolisci-stazione") == 0)
         {
            if(fscanf(pf, "%d", &distanza));
            root = elimina(root, distanza);
         }
         else if (strcmp(buffer, "rottama-auto") == 0)
         {
            if(fscanf(pf, "%d", &distanza));
            if(fscanf(pf, "%d", &autonomia));
            t = ricerca(root, distanza);
            if (t == NULL)
            {
               printf("non rottamata");
            }
            else
            {

               t->list = elimina_macchina(t, autonomia);
            }
         }

         else if (strcmp(buffer, "pianifica-percorso") == 0)
         {
            if(fscanf(pf, "%d", &partenza));
            if(fscanf(pf, "%d", &arrivo));
            nodo par = ricerca(root, partenza);
            nodo arr = ricerca(root, arrivo);
            if(partenza<arrivo)
               k=shortest_path(par, arr, arr);
            else if(partenza>arrivo)
             { 
               nodo_l lista=NULL;
               k=prima_passata(par, arr, &lista);
               if(k==0)
                  printf("nessun percorso");
               else 
               {
                  int z=0;
                  lista=seconda_passata(root, lista, &z);
                  while(z==1)
                  {
                   z=0;
                   lista=seconda_passata(root, lista, &z);
                  }

                  lista=lista_rovesciata(lista);
                  while(lista!=NULL)
                  {
                     if(lista->next==NULL)
                       printf("%d",lista->value);
                     else 
                      printf("%d ",lista->value);
   
                     lista=lista->next;
                  }
                  elimina_lista_2(lista);
               }
             }
         }
         printf("\n");
         if(fscanf(pf, "%s", buffer));
      }
   }
   fclose(pf);
   //visita(root);
  
   return 0;
}

nodo inserisci(nodo root, int k, int *p)
{
   nodo nuovo_nodo;
   nodo pre = NULL;
   nodo cur = root;
   nuovo_nodo = malloc(sizeof(elem));
   nuovo_nodo->dist = k;
   nuovo_nodo->left = NULL;
   nuovo_nodo->right = NULL;
   nuovo_nodo->p= NULL;
   nuovo_nodo->list = NULL;
   nuovo_nodo->max = -1;
   while (cur != NULL)
   {
      pre = cur;
      if (cur->dist > k)
      {
         cur = cur->left;
      }
      else if (cur->dist < k)
      {
         cur = cur->right;
      }
      else
      {
         printf("non aggiunta");
         *p = 1;
         return root;
      }
   }
   nuovo_nodo->p = pre;
   if (pre == NULL)
   {
      root = nuovo_nodo;
   }
   else if (k < pre->dist)
   {
      pre->left = nuovo_nodo;
   }
   else
   {
      pre->right = nuovo_nodo;
   }
   printf("aggiunta");
   return root;
}
void visita(nodo t)
{
   if (t == NULL)
   {
      return;
   }
   visita(t->left);
   printf("distanza: %d ", t->dist);
   /* if (cur == NULL)
   {
      printf("no macchine\n");
   }
   while (cur != NULL)
   {

        printf("%d - ", cur->value);
      cur = cur->next;
   }
   */
   printf("max: %d \n", t->max);
   visita(t->right);
   return;
}
nodo ricerca(nodo t, int k)
{
   if (t == NULL || t->dist == k)
   {
      return t;
   }
   else if (t->dist > k)
   {
      return ricerca(t->left, k);
   }
   else
   {
      return ricerca(t->right, k);
   }
}
nodo max(nodo t)
{
   nodo cur = t;
   while (cur->right != NULL)
   {
      cur = cur->right;
   }
   return cur;
}
nodo min(nodo t)
{
   nodo cur = t;
   while (cur->left != NULL)
   {
      cur = cur->left;
   }
   return cur;
}
nodo predecessore(nodo t)
{
   if (t->left != NULL)
   {
      return max(t->left);
   }
   nodo y = t->p;
   while (y != NULL && y->left == t)
   {
      t = y;
      y = y->p;
   }
   return y;
}
nodo successore(nodo t)
{
   if (t->right != NULL)
   {
      return min(t->right);
   }
   nodo y = t->p;
   while (y != NULL && y->right == t)
   {
      t = y;
      y = y->p;
   }
   return y;
}
nodo elimina(nodo root, int k)
{
   nodo x = ricerca(root, k);
   if (x == NULL)
   {
      printf("non demolita");
      return root;
   }
   else
      printf("demolita");
   nodo da_canc;
   nodo sottoa;
   if (x->left == NULL || x->right == NULL)
      da_canc = x;
   else
      da_canc = successore(x);
   if (da_canc->left != NULL)
      sottoa = da_canc->left;
   else
      sottoa = da_canc->right;
   if (sottoa != NULL)
      sottoa->p = da_canc->p;
   if (da_canc->p == NULL)
      root = sottoa;
   else if (da_canc == da_canc->p->left)
      da_canc->p->left = sottoa;
   else
      da_canc->p->right = sottoa;
    if (da_canc != x)
     {
        elimina_lista(x);
        x->dist = da_canc->dist;
        x->max=da_canc->max;
        x->list=da_canc->list;
        da_canc->list=NULL;
        free(da_canc);
     }
     else if(da_canc==x)
      {
          elimina_lista(da_canc);
          free(da_canc);
      }
   return root;
}
nodo_l aggiungimacchina(nodo t, int k)
{
   if (k > t->max)
      t->max = k;
   nodo_l cur;
   cur = malloc(sizeof(elem_2));
   cur->value = k;
   cur->next = t->list;
   return cur;
}
int trova_massimo(nodo_l l)
{
   int max = -1;
   nodo_l cur = l;
   while (cur != NULL)
   {
      if (cur->value > max)
         max = cur->value;
      cur = cur->next;
   }
   return max;
}
nodo_l elimina_macchina(nodo t, int k)
{
   nodo_l cur = t->list;
   nodo_l prec = NULL;
   int val;
   while (cur != NULL && cur->value != k)
   {
      prec = cur;
      cur = cur->next;
   }
   if(cur!=NULL)
   {
      printf("rottamata");
       val=cur->value;
      if (prec == NULL)
      {
         t->list = cur->next;
         free(cur);
      }
      else
      {
         prec->next = cur->next;
         free(cur);
      }
      if(val==t->max)
        {
          t->max = trova_massimo(t->list);

        }
   }
   else 
     printf("non rottamata");
     
   return t->list;
}
void stampa_macchine(nodo t)
{
   nodo_l cur = t->list;
   while (cur != NULL)
   {
      printf("%d\n", cur->value);
      cur = cur->next;
   }
}
void elimina_lista(nodo t)
{
   nodo_l temp;
   nodo_l lista = t->list;
   while (lista != NULL)
   {
      temp = lista->next;
      free(lista);
      lista = temp;
   }
   return;
}
int shortest_path(nodo partenza, nodo destinazione, nodo original_dest)
{
   nodo cur = destinazione;
   nodo min;
   int trovato = 0;
   if(partenza==destinazione)
   {
      printf("%d ", partenza->dist);
      return 1;
   }
   while (cur != partenza)
   {
      cur = predecessore(cur);
      if (cur->dist + cur->max >= destinazione->dist)
      {
         min = cur;
         trovato = 1;
      }
   }
   if (trovato == 0)
   {
      printf("nessun percorso");
      return 0;
   }
   else
      { 
        if(shortest_path(partenza, min, original_dest)==1)
          {
            if(destinazione==original_dest)
               printf("%d", destinazione->dist);
            else
              printf("%d ", destinazione->dist);
            return 1;
          }
        else 
            return 0;
      }

}
nodo nodoragg(nodo n)
{
   
  nodo cur=n;
  while(predecessore(cur)!=NULL && n->dist-n->max<=predecessore(cur)->dist)
  {
     cur=predecessore(cur);
  }
  if(cur==n || cur==NULL)
     return NULL;
   return cur;
}
nodo_l aggiungitappa(nodo_l ts, int v)
{
   nodo_l cur;
   cur = malloc(sizeof(elem_2));
   cur->value = v;
   cur->next = NULL;
   nodo_l app=ts;
   if(app==NULL)
    return cur;
   while(app->next!=NULL)
    {
      app=app->next;
    }
    app->next=cur;
    return ts;
}
int prima_passata(nodo partenza, nodo destinazione, nodo_l *lista)
{
   if(partenza==destinazione)
   {
       *lista=aggiungitappa(*lista, partenza->dist);
       return 1;
   }
   nodo k=shortest_path_2(partenza, destinazione);
    if(k==NULL)
     return 0;
    else{
        if(prima_passata(k, destinazione, lista)==1)
           { 
             *lista=aggiungitappa(*lista, partenza->dist);
             return 1;
           }
         else 
             return 0;
         }

}

nodo shortest_path_2(nodo partenza, nodo destinazione)
{
   
  
   nodo cur=predecessore(partenza);
   nodo min;
   int dist_max=partenza->dist;
   nodo ragg;
   int trovato=0;
   if(partenza==destinazione)
   {
      return partenza;
   }
   if(cur==destinazione)
    {
      if(partenza->dist-partenza->max<=cur->dist)
        return destinazione;
      else 
        return NULL;
    }
   while(cur!=NULL && cur->dist>=destinazione->dist && partenza->dist-partenza->max<=cur->dist)
   {
      
      ragg=nodoragg(cur);
      if(ragg!=NULL && (ragg->dist<=dist_max || ragg->dist<=destinazione->dist))
         { 
               dist_max=ragg->dist;
               min=cur;
               trovato=1;
         }
      cur=predecessore(cur);
   }
   if(trovato==0)
   {
       return NULL;
   }
   else 
     {
       return min;
     }
}
void elimina_lista_2(nodo_l lista)
{
 nodo_l temp;
 temp=lista;
   while (lista != NULL)
   {
      temp = lista->next;
      free(lista);
      lista = temp;
   }
   return;
}
nodo_l seconda_passata(nodo root, nodo_l lis, int *p)
{
  nodo_l n_1=lis;
  nodo_l n_2=lis->next;
  if(n_2==NULL)
   return lis;
  nodo_l n_3=lis->next->next;
  if(n_3==NULL)
   return lis;
  nodo nodo1=ricerca(root, n_1->value);
  nodo nodo2=ricerca(root, n_2->value);
  nodo nodo3=ricerca(root, n_3->value);
  nodo cur=nodo2;
  while(predecessore(nodo2)!=NULL && nodo3->dist-nodo3->max<=predecessore(nodo2)->dist && predecessore(nodo2)->dist>=nodo1->dist)
   {
      if(predecessore(nodo2)->dist-predecessore(nodo2)->max<=nodo1->dist)
      {  
         if(*p==0)
         *p=1;
         cur=predecessore(nodo2);
      }
      nodo2=predecessore(nodo2);
   }
   lis->next->value=cur->dist;
   lis->next=seconda_passata(root, lis->next, p);
   return lis;

}
nodo_l lista_rovesciata(nodo_l lista)
{

	nodo_l sec, coda, tmp;

	coda = lista;
	sec = lista->next;
	coda->next = NULL;
	tmp = coda;

	while (sec != NULL)
	{
		coda = sec;
		sec = sec->next;
		coda->next = tmp;
		tmp = coda;
	}

	return coda;
}
