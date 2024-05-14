/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER XZAO000.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques - Interface>
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Oct 1994	: Creation
* volcic	25 Jan 1994	: Modif Verif Nbre pour negatifs (v1.2)
* JPL		30/11/10 : Migration architecture HP ia64 (DEM 961) : suppression erreurs mineures  1.3
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : adequation format scanf et type d'entier  1.4
* JPL		26/04/17 : Changement de certains niveaux de trace  1.5
* JPL		07/02/23 : Augmentation de taille du tampon de lecture d'une ligne de fichier  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */
  
/* definition de macro locales */

/* declaration de variables locales */

static char *version = "xzaO000.c 1.6" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lecture d'une ligne d'un fichier ASCII
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


int XZAO000_LireLigneFich (	FILE  		*va_Fd,
                        	char 		*pa_Ligne,
                        	XDY_Entier  	va_NbCar,
                        	XDY_Entier  	*va_Resultat_out)

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Entier		va_Fd
* XDY_Entier            va_NbCar
*
* ARGUMENTS EN SORTIE :	
*
* char  		*pa_Ligne
*
* CODE RETOUR :
*
* 1				XDC_NOK
* EOF  				XZAO_EOF
* nombre de caracteres lus	vl_LongLigne  
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*    Fonction permettant de XZAO000_lire une ligne d'une fichier
*
* MECANISMES :
*
------------------------------------------------------*/

{
   char			pl_Ligne[400];
   XDY_Entier		vl_Size = sizeof (pl_Ligne);

    *va_Resultat_out = NULL;
    pl_Ligne[0] = XDC_FIN_CHN;
  
    if ( va_NbCar < sizeof ( pl_Ligne ) )
    {
       vl_Size = va_NbCar;
    }
   
    if ( fgets ( pl_Ligne, vl_Size, va_Fd ) == NULL )
    {
      if (feof ( va_Fd ))
      {   
         *va_Resultat_out = XZAO_EOF;
  
         return (XDC_OK);
      }
      
      else   
      
      {   
         XZST_03EcritureTrace(XZSTC_WARNING,"XZAO000_LireLigneFich: chaine null");
	 XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : chaine =  %s", pl_Ligne);
	 return (XDC_NOK);
      }
   }   
   else	 
   
   {
      strcpy ( pa_Ligne, pl_Ligne );
   }
          
   return (XDC_OK);
  
   
    
 }  



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Verification de la coherence des chaines de caracteres des fichiers
*    de configuration de la base
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO000_Verif_Coherence_String ( char *pa_String, XDY_Entier va_XDC_Type, char *pa_String_out ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* char *pa_string
*
* ARGUMENTS EN SORTIE :	
* char *pa_string_out
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

{
   int i;
   XDY_Entier vl_retcode = 0;
   XDY_Booleen vl_finligne;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN : XZAO000_Verif_Coherence_String : String = %s et LongueurType = %d", pa_String, va_XDC_Type );
   
   /*A
   ** On verifie que la chaine ne possede pas de caractere "\n" 
   */
   
   vl_finligne = XDC_FAUX;
   
   for ( i = 0; i < strlen ( pa_String ) ; i ++ )
   {
       if ( pa_String[i] == XZAO_LINE_FEED )
       {
          vl_finligne = XDC_VRAI;
          break;
       }
   }          
   
   if ( vl_finligne == XDC_FAUX ) 
   {
      /*B
      ** Si la longueur de la chaine depasse la longueur du type de chaine
      ** Erreur de saisie : chaine trop longue
      ** return ( XDC_NOK ) and pa_String_out = null
      */
      
      if ( strlen ( pa_String ) > va_XDC_Type )
      {
         /*cgcd_BilanConf_Trace( pa_String );*/
         printf ("\n***  %s  ***\n", pa_String );
         return ( XDC_NOK );
      } 
      
      else
      
      /*B
      ** Si la longueur de la chaine ne depasse pas la longueur du type de chaine
      ** return ( XDC_OK ) and pa_String_out = pa_String
      */
      
      {
          if ( strlen ( pa_String ) == 0 )
          {
             /*cgcd_BilanConf_Trace( pa_String );*/
             strcpy ( pa_String_out, " ");
             return ( XDC_OK );
          } 
          strcpy ( pa_String_out, pa_String );
          XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO000_Verif_Coherence_String retourne %d et String_out = %s", XDC_OK, pa_String_out);
 	  return ( XDC_OK );
      } 
   }
   
   else
   
   /*A
   ** La chaine possede le caractere "\n" 
   */
   
   {
      /*B
      ** Si la longueur de la chaine depasse la longueur du type de chaine + 1 
      ** Erreur de saisie : chaine trop longue
      ** return ( XDC_NOK ) and pa_String_out = null
      */
      
      if ( strlen ( pa_String ) > ( va_XDC_Type + 1 ) )
      {
         printf ("\n***  %s  ***\n", pa_String );
         /* cgcd_BilanConf_Trace( pa_String ); */
         return ( XDC_NOK );
      }  
      
      else
      
      /*B
      ** Si la longueur de la chaine ne depasse pas la longueur du type de chaine + 1
      ** return ( XDC_OK ) and pa_String_out = pa_String
      */
      
      {
          if ( strlen ( pa_String ) == 1 )
          {
             /*cgcd_BilanConf_Trace( pa_String );*/
             strcpy ( pa_String_out, " ");
             return ( XDC_OK );
          } 
          strncpy ( pa_String_out, pa_String, strlen ( pa_String ) - 1 );
          pa_String_out [ strlen ( pa_String ) - 1] = XZAO_FIN_STRING;
          XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO000_Verif_Coherence_String retourne %d et String_out = %s", XDC_OK, pa_String_out);
 	  return ( XDC_OK );
      }
   }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Verification de la coherence des chaines de caracteres des fichiers
*    de configuration de la base pour un entier, un long, etc...
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO000_Verif_Coherence_Nombre ( char *pa_String, XDY_Entier va_XZAO_Type, char *pa_String_out ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* char *pa_string
*
* ARGUMENTS EN SORTIE :	
*
* char *pa_string_out
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

{
   XDY_Entier  	vl_retcode = 0;
   long		vl_valeur_nbre = 0;
   XDY_Entier	i = 0;
   XDY_Entier	j = 0;
   XDY_Booleen  vl_finligne;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN : XZAO000_Verif_Coherence_Nombre : String = %s et LongueurType = %d", pa_String, va_XZAO_Type );
   
   /*A
   ** On verifie que la chaine ne possede pas de caractere "\n" 
   */
   
   vl_finligne = XDC_FAUX;
   
   for ( j = 0; j < strlen ( pa_String ) ; j ++ )
   {
       if ( pa_String[j] == XZAO_LINE_FEED )
       {
          vl_finligne = XDC_VRAI;
          break;
       }
   }          
   
   
   if ( vl_finligne == XDC_FAUX ) 
   {
      /*B
      ** Si la longueur de la chaine depasse la longueur du type de chaine
      ** Erreur de saisie : chaine trop longue
      ** return ( XDC_NOK ) and pa_String_out = null
      */
      
      if ( strlen ( pa_String ) >  va_XZAO_Type )
      {
         /*cgcd_BilanConf_Trace( pa_String );*/
         return ( XDC_NOK );
      } 
      
      else
      
      /*B
      ** Si les caracteres trouves ne sont pas compris entre 0 et 9
      ** Erreur de saisie : chaine trop longue
      ** return ( XDC_NOK ) and pa_String_out = null
      */
      
      while ( pa_String[i] != XZAO_FIN_STRING )
      {
         
         if ( ( ( pa_String[i] < '0' ) || ( pa_String[i] > '9' ) ) &&  pa_String[0] != '-' ) 
         {
            /*cgcd_BilanConf_Trace( pa_String );*/
            return ( XDC_NOK );
         } 
         
         i++;  
              
      }
      
      /*B
      ** Si OK : mise au format long de la valeur de pa_String
      ** Verification en fonction du type pour ne pas depasser la valeur maximale
      */
      
      sscanf ( pa_String, "%ld", &vl_valeur_nbre );
      
      switch ( va_XZAO_Type )
      {
      	case XZAO_LONG_UCHAR : 	if ( vl_valeur_nbre > XZAO_MAX_UCHAR )
      			       	{
      			           XZST_03EcritureTrace(XZSTC_WARNING,"XZAO000_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZAO_MAX_UCHAR );
      			           return ( XDC_NOK );
   		               	}
   		               	break;
      
       case XZAO_LONG_INT :    	if ( vl_valeur_nbre > XZAO_MAX_INT )
      			  	{
      			           XZST_03EcritureTrace(XZSTC_WARNING,"XZAO000_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZAO_MAX_INT );
      			           return ( XDC_NOK );
   		          	}
   		         	 break;
   		          
       case XZAO_LONG_LONG :   	if ( vl_valeur_nbre > XZAO_MAX_LONG )
      			  	{
      			     	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAO000_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZAO_MAX_LONG );
      			     	   return ( XDC_NOK );
   		          	}
   		          	break;
      
       case XZAO_LONG_BIT :    	if ( vl_valeur_nbre > XZAO_MAX_BIT )
      			  	{
      			           XZST_03EcritureTrace(XZSTC_WARNING,"XZAO000_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZAO_MAX_INT );
      			           return ( XDC_NOK );
   		          	}
   		         	break;
   		          
       
      
      }
      
      strcpy ( pa_String_out, pa_String );
   }
   
   else
   
   /*A
   ** La chaine possede le caractere "\n" 
   */
   
   {
      /*B
      ** Si la longueur de la chaine depasse la longueur du type de chaine + 1 
      ** Erreur de saisie : chaine trop longue
      ** return ( XDC_NOK ) and pa_String_out = null
      */
      
      if ( strlen ( pa_String ) > ( va_XZAO_Type + 1 ) )
      {
         printf ("\n***  %s  ***\n", pa_String );
            /*cgcd_BilanConf_Trace( pa_String );*/
         return ( XDC_NOK );
      } 
      
      else
      
      /*B
      ** Si les caracteres trouves ne sont pas compris entre 0 et 9
      ** Erreur de saisie : chaine trop longue
      ** return ( XDC_NOK ) and pa_String_out = null
      */
      
      while ( pa_String[i] != XZAO_LINE_FEED )
      {
         
         if ( ( ( pa_String[i] < '0' ) || ( pa_String[i] > '9' ) ) &&  pa_String[0] != '-') 
         {
            printf ("\n***  %s  ***\n", pa_String );
            /*cgcd_BilanConf_Trace( pa_String );*/
            return ( XDC_NOK );
         } 
         
         i++;  
              
      }
      
      /*B
      ** Si OK : mise au format long de la valeur de pa_String
      ** Verification en fonction du type pour ne pas depasser la valeur maximale
      */
      
      sscanf ( pa_String, "%ld", &vl_valeur_nbre );
      
      switch ( va_XZAO_Type )
      {
      	case XZAO_LONG_UCHAR : 	if ( vl_valeur_nbre > XZAO_MAX_UCHAR )
      			  	{
      			    	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAO000_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZAO_MAX_UCHAR );
      			    	   return ( XDC_NOK );
   		          	}
   		          	break;
      
       case XZAO_LONG_INT :    	if ( vl_valeur_nbre > XZAO_MAX_INT )
      			  	{
      			     	   XZST_03EcritureTrace(XZSTC_WARNING,"XZAO000_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZAO_MAX_INT );
      			     	   return ( XDC_NOK );
   		          	}
   		          	break;
   		          
       case XZAO_LONG_LONG :   if ( vl_valeur_nbre > XZAO_MAX_LONG )
      			  	{
      			    	    XZST_03EcritureTrace(XZSTC_WARNING,"XZAO000_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZAO_MAX_LONG );
      			     	   return ( XDC_NOK );
   		          	}
   		         	 break;
       
       case XZAO_LONG_BIT :    	if ( vl_valeur_nbre > XZAO_MAX_BIT )
      			  	{
      			           XZST_03EcritureTrace(XZSTC_WARNING,"XZAO000_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZAO_MAX_INT );
      			           return ( XDC_NOK );
   		          	}
   		         	break;
      }
      
      	  strncpy ( pa_String_out, pa_String, strlen ( pa_String ) - 1 );
          pa_String_out [ strlen ( pa_String ) - 1] = XZAO_FIN_STRING;
       
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAO000_Verif_Coherence_Nombre retourne %d et String_out = %s", XDC_OK, pa_String_out);
   return ( XDC_OK );
     
}  
