/*E*/
/*  Fichier : $Id: xdea_eqn_alt.c,v 1.8 2012/07/04 17:47:54 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2012/07/04 17:47:54 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdea * FICHIER xdea_eqn_alt.c
******************************************************
* DESCRIPTION DU MODULE :
*
*       calcul des equations d'alerte en polonaise inverse
*
******************************************************
* HISTORIQUE :
*
* Gaborit   23/01/95 : creation
* Gaborit   03/02/95 : complement trace (1.2)
* Gaborit   03/02/95 : modif test == et non = (1.3)
* Gaborit   03/02/95 : complement trace (1.4)
* Niepceron 05/11/96 : Ajou du calcul niv tfm (dem/1248) v1.5
	    10/03/97 : Correction pour core		v1.6
*JMG   13/11/07        : ajout site de gestion DEM715
* JMG	04/07/12 	: correction linux 
********************************************************/

/* fichiers inclus */

#include "xdea_eqn_alt.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xdea_eqn_alt.c,v 1.8 2012/07/04 17:47:54 pc2dpdy Exp $ : xdea_eqn_alt" ;

/* declaration de fonctions locales */
int xdea_lire_txt (FILE *,
                   XDY_TexteAlerte,
                   XDY_Octet *);

int xdea_lire_arg(FILE *,
                  XDEAT_VAR *,
                  int,
                  char * ,
                  double *,
                  int * );

/*X------------------------------------------------------
* SERVICE RENDU :
*
* evalue les equations a partir d'un fichier pass≈ en argument
* et des variables pass≈es dans le tableau pa_variables_in
* Pour chaque equation vraie, appelle xzaa01
*
* si va_Test_in <> XDC_NON, on teste la syntaxe, c'est a dire
* que l'on deroule les equations sans creer d'alerte, et sans
* s'arreter sur les divisions par zero.
* les erreurs eventuelles sont tracees
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xdea01_evaluer ( FILE *          pa_file_in,
                   XDEAT_VAR *     pa_variables_in,
                   int             va_nb_variables_in,
                   XDY_Horodate    va_Horodate_in,
                   XDY_TypeEqt     va_TypeEqt_in,
                   XDY_Eqt         va_Eqt_in,
                   XDY_Booleen	   va_Test_in,
                   XDY_Octet	   *pa_nivTFM)
/*
* ARGUMENTS EN ENTREE :
*
* FILE *          pa_file_in           fichier d'≈quations
* XDEAT_VAR *     pa_variables_in      tableau des variables
* int             va_nb_variables_in   nombre de variables dans le tableau
* XDY_Horodate    va_Horodate
* XDY_TypeEqt     va_TypeEqt_in
* XDY_Eqt         va_Eqt_in
* XDY_Octet         *pa_nivTFM
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*
*  XDC_OK
*  XDC_NOK
*
* CONDITION D'UTILISATION :
* syntaxe du fichier d'alerte :
*	<COMMENTAIRES>
*	<TEXTE ALERTE>
*	<VALEUR> ou <OPERATEUR> ou <COMMENTAIRES>
*	...
*	<FIN EQUATION>
*	etc...
* avec :
*	<COMMENTAIRES>=	<retour chariot> ou
*			# texte de commentaire <retour chariot>
*
*	<TEXTE ALERTE>= texte de l'alerte (tronqu≈ a 100 car) <retour chariot>
*
*	<VALEUR>=	constante commencant par 0..9 ou - (negatif) <retour chariot> ou
*			variable commencant par A..Z d'au moins 2 car <retour chariot>
*
*	<OPERATEUR>=	+ <retour chariot>	depile 2 valeur et empile la somme
*			- <retour chariot>	depile 2 valeur et empile la difference
*			* <retour chariot>	depile 2 valeur et empile le produit
*			/ <retour chariot>	depile 2 valeur et empile le quotient
*			> <retour chariot>	depile 2 valeur et empile 0 (faux) ou 1 (vrai)
*			< <retour chariot>	depile 2 valeur et empile 0 (faux) ou 1 (vrai)
*			= <retour chariot>	depile 2 valeur et empile 0 (faux) ou 1 (vrai)
*			! <retour chariot>	duplique la valeur au sommet de la pile
*			S <espace> 0..9 <retour chariot>
*						depile une valeur et la met dans
*						le registre correspondant (stable pour le fichier)
*			R <espace> 0..9 <retour chariot>
*						empile la valeur du registre correspondant
*
*	<FIN EQUATION>=	$ texte de commentaire <retour chariot>
*						depile la derniere valeur de la pile et 
*						declenche l'alerte si elle est <> 0
*						Remarque : la pile doit etre vide !
*
* INTERFACE :
*
* MECANISMES :
*
------------------------------------------------------*/

{
   static char     *version = "$Id: xdea_eqn_alt.c,v 1.8 2012/07/04 17:47:54 pc2dpdy Exp $ : xdea01_evaluer";
   XDY_TexteAlerte  pl_texte;
   char             vl_oper;
   double           vl_valeur;
   int              vl_indice;
   int              vl_pos_pile;
   double           vl_pile[XDEAC_pile];
   double           vl_registre[XDEAC_registre];
   int		    vl_fin;
   int		    vl_calcterm=XDC_FAUX;
   
   
   XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s", version);
   
   if (pa_nivTFM!=NULL)
      *pa_nivTFM=(XDY_Octet) -1;

   /*A Boucle sur les equations du fichier */
   while ((vl_calcterm==XDC_FAUX) && (xdea_lire_txt(pa_file_in,pl_texte, pa_nivTFM)==XDC_OK) )
   {
      /*A Initialisation des valeurs de l'equation */
      vl_pos_pile=-1; /* pile vide */
      vl_oper=' '; /* pas d'operateur */
      vl_fin=0; /* indicateur de fin d'equation */
      
     /*A boucle de lecture de l'equation */
     while (vl_fin==0)
     {
      
      /*A Lecture des arguments de l'≈quation */
      if (xdea_lire_arg(pa_file_in,pa_variables_in,va_nb_variables_in,&vl_oper,&vl_valeur,&vl_indice)!=XDC_OK)
      {
         XZST_03EcritureTrace(XZSTC_WARNING,"%s erreur a la lecture de l'equation :%s", version,pl_texte);
         XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK ", version);
         return XDC_NOK;
      }
      XZST_03EcritureTrace(XZSTC_DEBUG3,"%s a lu \noperateur : %c\nvaleur : %lf\nindice : %d\npile= %d",version,vl_oper,vl_valeur,vl_indice,vl_pos_pile);
      switch (vl_oper)
      {
         case '+' : if (vl_pos_pile<1)
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile vide pour + dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_pile[vl_pos_pile-1]=vl_pile[vl_pos_pile-1]+vl_pile[vl_pos_pile];
                       vl_pos_pile--;
                    }
                    break;
            
         case '*' : if (vl_pos_pile<1)
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile vide pour * dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_pile[vl_pos_pile-1]=vl_pile[vl_pos_pile-1]*vl_pile[vl_pos_pile];
                       vl_pos_pile--;
                    }
                    break;
            
         case '-' : if (vl_pos_pile<1)
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile vide pour - dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_pile[vl_pos_pile-1]=vl_pile[vl_pos_pile-1]-vl_pile[vl_pos_pile];
                       vl_pos_pile--;
                    }
                    break;
            
         case '/' : if (vl_pos_pile<1)
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile vide pour - dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else if (vl_pile[vl_pos_pile]==0)
                    {
                       if (va_Test_in==XDC_NON)
                       {
                         XZST_03EcritureTrace(XZSTC_WARNING,"%s division par zero dans l'equation :%s", version,pl_texte);
                         vl_fin=1;
                       }
                       else
                       {
                         vl_pos_pile--; /* en mode test, on n'effectue pas la division */
                       }
                    }
                    else
                    {
                       vl_pile[vl_pos_pile-1]=vl_pile[vl_pos_pile-1]/vl_pile[vl_pos_pile];
                       vl_pos_pile--;
                    }
                    break;
            
         case '>' : if (vl_pos_pile<1)
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile vide pour > dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_pile[vl_pos_pile-1]=(vl_pile[vl_pos_pile-1]>vl_pile[vl_pos_pile])?1:0;
                       vl_pos_pile--;
                    }
                    break;
   
         case '<' : if (vl_pos_pile<1)
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile vide pour < dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_pile[vl_pos_pile-1]=(vl_pile[vl_pos_pile-1]<vl_pile[vl_pos_pile])?1:0;
                       vl_pos_pile--;
                    }
                    break;
   
         case '=' : if (vl_pos_pile<1)
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile vide pour = dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_pile[vl_pos_pile-1]=(vl_pile[vl_pos_pile-1]=vl_pile[vl_pos_pile])?1:0;
                       vl_pos_pile--;
                    }
                    break;
   
         case '!' : if (vl_pos_pile<0) /* duplique sommet de la pile */
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile vide pour ! dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else if (vl_pos_pile>=(XDEAC_pile-1))
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile pleine pour ! dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_pile[vl_pos_pile+1]=vl_pile[vl_pos_pile];
                       vl_pos_pile++;
                    }
                    break;
                    
         case 'S' : if (vl_pos_pile<0) /* stocke sommet de la pile dans registre et depile */
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile vide pour S dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else if ((vl_indice<0)||(vl_indice>=XDEAC_registre))
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s registre errone pour S dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_registre[vl_indice]=vl_pile[vl_pos_pile];
                       vl_pos_pile--;
                    }
                    break;
                    
         case 'R' : if (vl_pos_pile>=(XDEAC_pile-1)) /* restaure le registre au sommet de la pile */
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile pleine pour R dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else if ((vl_indice<0)||(vl_indice>=XDEAC_registre))
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s registre errone pour R dans l'equation :%s", version,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_pile[vl_pos_pile+1]=vl_registre[vl_indice];
                       vl_pos_pile++;
                    }
                    break;
   
         case 'V' : if (vl_pos_pile>=(XDEAC_pile-1)) /* met la valeur au sommet de la pile */
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile pleine pour valeur %lf dans l'equation :%s", version,vl_valeur,pl_texte);
                       vl_fin=1;
                    }
                    else
                    {
                       vl_pile[vl_pos_pile+1]=vl_valeur;
                       vl_pos_pile++;
                    }
                    break;
   
         case '$' : if (vl_pos_pile!=0) /* fin d'equation : envoie alerte si pile!= 0 */
                    {
                    if (pa_nivTFM!=NULL)
                    	*pa_nivTFM=(XDY_Octet)  -1;
                       XZST_03EcritureTrace(XZSTC_WARNING,"%s pile non vide en fin d'equation :%s", version,pl_texte);
                    }
                    else if ((vl_pile[0]!=0)&&(va_Test_in==XDC_NON)&&(pa_nivTFM==NULL))
                    {
                       if (xzaa01_Creer_Alerte (va_Horodate_in,va_TypeEqt_in,va_Eqt_in,XZAAC_EQA_EVT,pl_texte, 0)!=XDC_OK)
                       {
                          XZST_03EcritureTrace(XZSTC_WARNING,"%s xzaac01 retourne NOK pour l'equation :%s", version,pl_texte);
                       }
                       else
                       {
                          XZST_03EcritureTrace(XZSTC_INFO,"%s xzaac01 retourne OK pour l'equation :%s", version,pl_texte);
                       }
                    }
                    else
                    {
                    if (pa_nivTFM!=NULL)
		       {
		       *pa_nivTFM=(XDY_Octet) vl_pile[0];
		       XZST_03EcritureTrace(XZSTC_INFO,"%s  retourne %d pour l'equation ", version,(int) *pa_nivTFM);
		       }
		    vl_calcterm=XDC_VRAI;                    
                    }
                    
                    vl_fin=1;
                    break;
   
         default  : XZST_03EcritureTrace(XZSTC_WARNING,"%s operateur invalide dans :%s", version,vl_valeur,pl_texte);
                    vl_fin=1;
                    break;
      } /* Fin switch */
      if (vl_oper!='$') /* on a eu une fin anormale donc recherche du $ suivant */
      {
         
      }
     } /* Fin boucle de lecture de l'equation */
   } /* Fin boucle sur les equations du fichier */
   XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne OK ", version);
   return XDC_OK;
}

/*X------------------------------------------------------
* SERVICE RENDU :
*
* lit le texte d'une equation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xdea_lire_txt (FILE *          pa_file_in,
                   XDY_TexteAlerte pa_texte_out,
                   XDY_Octet       *pa_nivTFM )
/*
* ARGUMENTS EN ENTREE :
*
* FILE *          pa_file_in           fichier d'≈quations
*
* ARGUMENTS EN SORTIE :
*
* XDY_TexteAlerte pa_texte_out
*
* CODE RETOUR :
*
*  XDC_OK
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* MECANISMES :
*
------------------------------------------------------*/
{
   static char     *version = "$Id: xdea_eqn_alt.c,v 1.8 2012/07/04 17:47:54 pc2dpdy Exp $ : xdea_lire_txt";
   char pl_texte[1001];
   
   XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s", version);
   
   /*A initialisation du texte a chaine vide */
   *pa_texte_out='\0';
   /*A initialisation du texte lu a commentaire */
   *pl_texte='#';
   
   if (pa_file_in==NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"%s fichier = NULL", version);
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK ", version);
      return XDC_NOK;
   }
   /*A Si c'est une equation pour niv TFM alors on ne lit pas le texte d'alerte */
   if ( pa_nivTFM == NULL )
   {
   /*A on saute les commentaires et les lignes vides */
   while ((*pl_texte=='#')||(*pl_texte=='\n'))
   {
   	if (fgets(pl_texte,1000,pa_file_in)==NULL)
   	{
      		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK (fin de fichier)", version);
      		return XDC_NOK;
   	}
   }
   /* on enleve le caractere fin de ligne */
   if (pl_texte[strlen(pl_texte)-1]=='\n')
   {
      pl_texte[strlen(pl_texte)-1]='\0';
   }
   /* on tronque a la longueur du texte alerte */
   strncpy(pa_texte_out,pl_texte,XDC_Lon_TexteAlerte-1);
   pa_texte_out[XDC_Lon_TexteAlerte-1]='\0';
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne OK ", version);
   return XDC_OK;
}

/*X------------------------------------------------------
* SERVICE RENDU :
*
* lit un argument d'une equation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xdea_lire_arg(FILE *          pa_file_in,
                  XDEAT_VAR *     pa_variables_in,
                  int             va_nb_variables_in,
                  char *          pa_oper_out,
                  double *        pa_valeur_out,
                  int *           pa_indice_out)
/*
* ARGUMENTS EN ENTREE :
*
* FILE *          pa_file_in           fichier d'≈quations
* XDEAT_VAR *     pa_variables_in
*
* ARGUMENTS EN SORTIE :
*
* char *          pa_oper_out
* double *        pa_valeur_out
* int *           pa_indice_out
*
* CODE RETOUR :
*
*  XDC_OK
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* INTERFACE :
*
* MECANISMES :
*
------------------------------------------------------*/
{
   static char     *version = "$Id: xdea_eqn_alt.c,v 1.8 2012/07/04 17:47:54 pc2dpdy Exp $ : xdea_lire_arg";
   char pl_texte[1001];
   char *pl_mot;
   int i;
   
   XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s", version);
   
   pl_mot=pl_texte;

   if (pa_file_in==NULL)
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"%s fichier = NULL", version);
      XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK ", version);
      return XDC_NOK;
   }

   /*A on saute les commentaires et les lignes vides */
	strcpy(pl_mot," ");
   while (pl_mot!=NULL)
   {
   	if (feof(pa_file_in)!=0)
   	{
      		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK (fin fichier)", version);
      		return XDC_NOK;
   	}
   	fgets(pl_texte,1000,pa_file_in);
   	if (ferror(pa_file_in)!=0)
   	{
      		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK (erreur lecture fichier)", version);
      		return XDC_NOK;
   	}
   	 XZST_03EcritureTrace(XZSTC_FONCTION,"fgets(pl_texte %s",pl_texte); 
   	/*A on prend le premier mot */
   	pl_mot=strtok(pl_texte," \t\n");
   	
	XZST_03EcritureTrace(XZSTC_FONCTION,"pl_mot %s",pl_mot);
   	if (pl_mot!=NULL)
   	{
   		switch(*pl_mot)
   		{
   			case '#' :	break; /* on saute le commentaire */
   			case '+' :
   			case '*' :
   			case '-' :
   			case '/' :
   			case '>' :
   			case '<' :
   			case '=' :
   			case '!' :	if (strlen(pl_mot)!=1)
   					/* le mot cle n'est pas isol≈ */
   					{
   					  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK (syntaxe incorrecte)", version);
   					  return XDC_NOK;
   					}
					/* on remonte l'op≈rateur */
   			case '$' :	*pa_oper_out=*pl_mot;
   					*pa_valeur_out=0.0;
   					*pa_indice_out=0;
   					pl_mot=NULL; /* on sort de la boucle */
   					break;
   			default  :	if ((strlen(pl_mot)==1)&&((*pl_mot=='S')||(*pl_mot=='R')))
   					/* il s'agit de l'operateur */
   					{
   						*pa_oper_out=*pl_mot;
   						*pa_valeur_out=0.0;
   						/* lecture de l'indice */
   						pl_mot=strtok(NULL," \t\n");
   						if (pl_mot!=NULL)
   						{
   						  *pa_indice_out=atoi(pl_mot);
   						  pl_mot=NULL; /* on sort de la boucle */
   						}
   						else
   						{
   						  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK (syntaxe incorrecte)", version);
						  return XDC_NOK;
   						}
   					}
   					else
   					/* il doit s'agir d'une valeur ou d'une variable */
   					if ((strlen(pl_mot)>1)&&(*pl_mot>='A')&&(*pl_mot<='Z'))
   					/* il s'agit d'une variable */
   					{
   					  for (i=0;(i<va_nb_variables_in)&&(pl_mot!=NULL);i++)
   					  {
   					    if (strcmp(pl_mot,pa_variables_in[i].nom)==0)
   					    /* on a trouve une variable */
   					    {
 					      XZST_03EcritureTrace(XZSTC_WARNING, " %s : varible no %d = %lf", version,pa_variables_in[i].valeur);
  					      *pa_oper_out='V';
   					      *pa_valeur_out=pa_variables_in[i].valeur;
   					      *pa_indice_out=0;
   					      pl_mot=NULL; /* on sort de la boucle */
   					    }
   					  }
   					  if (pl_mot!=NULL)
   					  /* on n'a pas reconnu la variable */
   					  {
   					   XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK (syntaxe incorrecte)", version);
					   return XDC_NOK;
   					  }
   					}
   					else
   					if (((*pl_mot>='0')&&(*pl_mot<='9'))||(*pl_mot=='-'))
   					/* il s'agit d'une valeur */
   					{
					  XZST_03EcritureTrace(XZSTC_FONCTION,"ici");
   					  sscanf(pl_mot,"%lf",pa_valeur_out);
					  XZST_03EcritureTrace(XZSTC_FONCTION,"ici");
   					  *pa_oper_out='V';
					  XZST_03EcritureTrace(XZSTC_FONCTION,"ici");
   					  *pa_indice_out=0;
					  XZST_03EcritureTrace(XZSTC_FONCTION,"ici");
   					  pl_mot=NULL; /* on sort de la boucle */
					  XZST_03EcritureTrace(XZSTC_WARNING, " %s : valeur num %lf", version,*pa_valeur_out);
   					}
   					else
   					/* il s'agit d'une erreur */
   					{
   					 XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne NOK (syntaxe incorrecte)", version);
					 return XDC_NOK;
					}
   		} /* fin switch */
   	} /* fin if pl_mot!=null */
   	else pl_mot=pl_texte; /* on a trouve une ligne vide */
   }
 /*  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : %s retourne OK  pa_oper_out=%s, pa_valeur=%lf, pa_indice_out=%d", version ,*pa_oper_out,*pa_valeur_out,*pa_indice_out);*/
   return XDC_OK;
}
