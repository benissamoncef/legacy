/*E*/  
/* Fichier : $Id: xzeh.c,v 1.10 2017/02/17 13:34:32 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 2017/02/17 13:34:32 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEH  * FICHIER xzeh.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Lecture de l'horodate du Centrale horaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	11 Aug 1994	: Creation
* Mismer.D	version 1.2	06 Dec 1994	: Envoi du siecle dans l'horodate
* Volcic.F	version 1.3	01 Mar 1995	:
* Mismer.D	version 1.4	02 Mar 1995	:
* Volcic.F	version 1.5	02 Mar 1995	:
* Fontaine.C	version 1.6	24 Mai 1995	: Reprise historique
* Mismer.D	version 1.7	02 Oct 1995	: Modif pour abort XZEH02 en cas de trame incorrecte
* Mismer.D	version 1.8	01 Dec 1995	: Correction pour abort XZEH02 en cas de trame incorrecte
* JPL		version 1.9	17/11/2010	: Suppression warnings a la compilation
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>
#include "xzeh.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzeh.c,v 1.10 2017/02/17 13:34:32 gesconf Exp $ : xzeh" ;
int	XZEHV_FD_HORLOGE = 0;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Ouverture de la ligne serie connecte au Centrale horaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEH01_Ouverture_Horloge ( char         *pa_Device, 
			       unsigned int va_Vitesse,
			       unsigned int va_NbitInfo,
			       unsigned int va_NbitStop,
			       unsigned int va_Parite)

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   pa_Horodate		Horodate donn�e par la centrale horaire
*
* CODE RETOUR : 
* XDC_OK 		Succ�s
* XZEHC_DEV_CFN		Configuration du device 
*			incorrecte.
*
* CONDITION D'UTILISATION
*   Est appelee une fois a l'initialisation afin de pouvoir lire l'horodate ulterieurement
*
* FONCTION 
*   Controle des parametre d'entree.
*   Ouverture et configuration de la liaison serie par appel de la primitive XZSS08
*   Memorisation du descripteur de fichier dans XZEHV_FD_HORLOGE
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzeh.c,v 1.10 2017/02/17 13:34:32 gesconf Exp $ : XZEH01_Ouverture_Horloge" ;
  int				vl_FlagParams = -1;

        XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZEH01_Ouverture_Horloge"  );
        XZST_03EcritureTrace ( XZSTC_WARNING, "IN  : XZEH01_Ouverture_Horloge %s", pa_Device  );
  
	/*A Controle des parametres d'entree */
	if ( ( va_NbitInfo == XZEHC_RS232_INFO8 ) &&
	     ( va_NbitStop == XZEHC_RS232_STOP1 ) &&
	     ( va_Parite == XZEHC_RS232_NOPAR ) )
	{ 
	   vl_FlagParams = XZSSC_8BITS_PNON_1SB;
	}
	if ( ( va_NbitInfo == XZEHC_RS232_INFO8 ) &&
	     ( va_NbitStop == XZEHC_RS232_STOP1 ) &&
	     ( va_Parite == XZEHC_RS232_PAIR ) )
	{ 
	   vl_FlagParams = XZSSC_8BITS_PPAIRE_1SB;
	}
	if ( ( va_NbitInfo == XZEHC_RS232_INFO8 ) &&
	     ( va_NbitStop == XZEHC_RS232_STOP1 ) &&
	     ( va_Parite == XZEHC_RS232_IMPAIR ) )
	{ 
	   vl_FlagParams = XZSSC_8BITS_PIMPAIRE_1SB;
	}
	if ( ( va_NbitInfo == XZEHC_RS232_INFO8 ) &&
	     ( va_NbitStop == XZEHC_RS232_STOP2 ) &&
	     ( va_Parite == XZEHC_RS232_IMPAIR ) )
	{ 
	   vl_FlagParams = XZSSC_8BITS_PNON_2SB;
	}
	if ( ( va_NbitInfo == XZEHC_RS232_INFO8 ) &&
	     ( va_NbitStop == XZEHC_RS232_STOP2 ) &&
	     ( va_Parite == XZEHC_RS232_PAIR ) )
	{ 
	   vl_FlagParams = XZSSC_8BITS_PPAIRE_2SB;
	}
	if ( ( va_NbitInfo == XZEHC_RS232_INFO8 ) &&
	     ( va_NbitStop == XZEHC_RS232_STOP2 ) &&
	     ( va_Parite == XZEHC_RS232_IMPAIR ) )
	{ 
	   vl_FlagParams = XZSSC_8BITS_PIMPAIRE_2SB;
	}
	 
	/*A Si parametre incorrect retourne NOK */
	if ( vl_FlagParams < 0 )
	{
	   XZST_03EcritureTrace ( XZSTC_WARNING, "OUT : XZEH01_Ouverture_Horloge parametres incorrect"  );
           return ( XDC_NOK );
        }

  	/*A Si parametre correcte Ouverture Liaison serie */
  	if ( XZSS_08OuvertureRS232 ( 	pa_Device, 
					va_Vitesse,
					vl_FlagParams,
					&XZEHV_FD_HORLOGE ) != XDC_OK )
	{
	   XZST_03EcritureTrace ( XZSTC_WARNING, "OUT : XZEH01_Ouverture_Horloge Ouverture RS Impossible"  );
	   return ( XDC_NOK );
	}
	else
	{
	   XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH01_Ouverture_Horloge Ouverture RS OK, fd=%d", XZEHV_FD_HORLOGE  );
	   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEH01_Ouverture_Horloge Ouverture RS OK"  );
	   return ( XDC_OK );
	}
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Lecture sur la ligne serie connecte au Centrale horaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEH02_Lecture_Horloge ( char         *pa_Horodate )

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   pa_Horodate		Horodate donn�e par la centrale horaire
*
* CODE RETOUR : 
* XDC_OK 		Succ�s
* XZEHC_DEV_CFN		Configuration du device 
*			incorrecte.
*
* CONDITION D'UTILISATION
*   Demande de lecture de l'horodate de la centrale 
*   horaire li�e au PC d'un site ou du CI
*
* FONCTION 
*   R�ception de l'horadate en r�ponse par appel � la
*   primitive de lecture liaison s�rie XZSS10.
*   Retour de la r�ponse dans pa_Horodate.
*
------------------------------------------------------*/
{
  static char *version = "$Id: xzeh.c,v 1.10 2017/02/17 13:34:32 gesconf Exp $ : XZEH02_Lecture_Horloge" ;

  char			vl_LectureRS[3*XZEHC_TAILLE_HORODATE];
  char			vl_Resultat[3*XZEHC_TAILLE_HORODATE];
  char			vl_Travail[3*XZEHC_TAILLE_HORODATE];
  char 			vl_Poub[512];
  char			*pl_Horodate = NULL;
  double		vl_Delta;
  double		vl_Floor;
  double		vl_Attente = (double) 2.0;
  struct tm  		vl_Time;
  time_t 		vl_HorodateSec;
  
  fd_set		vl_mask_read;
  struct  timeval	vl_timeout;

  
   XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZEH02_Lecture_Horloge"  );
   memset ( vl_Resultat, '\0',3*XZEHC_TAILLE_HORODATE-1 );
   
   /*A Init du masque pour select*/
   FD_ZERO ( &vl_mask_read );
   FD_SET  ( XZEHV_FD_HORLOGE, &vl_mask_read );
     
   
   /*A Attendre des messages en mode non bloquant */
   vl_timeout.tv_sec  = 0;
   vl_timeout.tv_usec = 10;
   
   sem_wait(&vg_semaphore); 
   while ( (select ( XZEHV_FD_HORLOGE + 1, &vl_mask_read, NULL, NULL, &vl_timeout )  != -1) )
   {
      /*A Si un �venement horloge */     
      if ( FD_ISSET ( XZEHV_FD_HORLOGE, &vl_mask_read ) ) 
      {
         if ( read ( XZEHV_FD_HORLOGE , vl_Poub, sizeof(vl_Poub)-1 ) == -1 )
         {
            sem_post(&vg_semaphore); 
            XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH02_Lecture_Horloge : Erreur sur un read Horloge  " );
            XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEH02_Lecture_Horloge avec NOK " );
            return ( XDC_NOK );
         }
	 vl_timeout.tv_sec  = 0;
         vl_timeout.tv_usec = 10;
      }
      else
      {
         break;
      }
   }
   sem_post(&vg_semaphore); 
 
   /*A Init du masque pour select*/
   FD_ZERO ( &vl_mask_read );
   FD_SET  ( XZEHV_FD_HORLOGE, &vl_mask_read );

   /*A Attendre des messages en mode non bloquant */
   vl_Floor = floor (vl_Attente);
   vl_timeout.tv_sec  = (unsigned long)vl_Floor;
   vl_timeout.tv_usec = (long)((vl_Attente - vl_Floor)*1000000);

   vl_Delta = TutGetWallTime();
   
   /* XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH02_Lecture_Horloge : Timeout=%ld,%ld - Attente=%lg", vl_timeout.tv_sec, vl_timeout.tv_usec, vl_Attente ); */
   
   sem_wait(&vg_semaphore); 
   while ( (select ( XZEHV_FD_HORLOGE + 1, &vl_mask_read, NULL, NULL, &vl_timeout )  != -1) )
   {
      vl_Attente -= (vl_Floor = TutGetWallTime()) - vl_Delta;
      vl_Delta = vl_Floor;
      vl_Floor = floor (vl_Attente);
      vl_timeout.tv_sec  = (unsigned long)vl_Floor;
      vl_timeout.tv_usec = (long)((vl_Attente - vl_Floor)*1000000);
   /* XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH02_Lecture_Horloge : Timeout=%ld,%ld - Attente=%lg", vl_timeout.tv_sec, vl_timeout.tv_usec, vl_Attente ); */

      /*A Si un �venement horloge */     
      if ( !FD_ISSET ( XZEHV_FD_HORLOGE, &vl_mask_read ) ) 
      {
         sem_post(&vg_semaphore); 
         XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH02_Lecture_Horloge : Pas de lecture Horloge correcte avant la fin du time-out de 2 secondes." );
         XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEH02_Lecture_Horloge avec NOK " );
         return ( XDC_NOK );
      }
      else
      { 
         XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH02_Lecture_Horloge : EVT HORLOGE " );
    
         memset ( vl_LectureRS, '\0',3*XZEHC_TAILLE_HORODATE-1 );
         if ( read ( XZEHV_FD_HORLOGE , vl_LectureRS, 2*XZEHC_TAILLE_HORODATE ) == -1 )
         {
            sem_post(&vg_semaphore); 
            XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH02_Lecture_Horloge : Erreur sur un read Horloge  " );
            XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEH02_Lecture_Horloge avec NOK " );
            return ( XDC_NOK );
         }
         else
         {
            /* XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH02_Lecture_Horloge : EVT LECTURE HORLOGE " ); */
            if ( strlen(vl_Resultat) + strlen(vl_LectureRS) > 3*XZEHC_TAILLE_HORODATE-1 )
            {
                XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur:%s%s", vl_Resultat, vl_LectureRS );
                memset ( vl_Resultat, '\0',3*XZEHC_TAILLE_HORODATE-1 );
            }
            strcat ( vl_Resultat, vl_LectureRS );
            strcpy ( vl_Travail, vl_Resultat );
            if ( ((pl_Horodate = strtok (vl_Travail, XZEHC_FIN_HORODATE )) != NULL ) &&
                 ( strlen (pl_Horodate) == XZEHC_TAILLE_HORODATE ) )
            {
               XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH02_Lecture_Horloge : EVT RESULTAT  HORLOGE : %s ", vl_Travail );

               /*A retour de l'horodate a l'appelant */
               if ( sscanf( pl_Horodate+4 ,"%02u/%02u/%02u %02u:%02u:%02u",
	  		&vl_Time.tm_mday,
	  		&vl_Time.tm_mon,
	  		&vl_Time.tm_year,
	  		&vl_Time.tm_hour,
	  		&vl_Time.tm_min,
	  	   	&vl_Time.tm_sec ) == 6)
	       {
   	           vl_Time.tm_isdst = 0 ;
	           vl_Time.tm_mon = vl_Time.tm_mon -1;
	           vl_Time.tm_year = (vl_Time.tm_year > 70) ? vl_Time.tm_year : vl_Time.tm_year+100;
	           if ( (vl_HorodateSec = mktime(&vl_Time)) != (time_t)-1 )
	           {
                 sem_post(&vg_semaphore); 
                      sprintf ( pa_Horodate, "%ld", vl_HorodateSec );
   XZST_03EcritureTrace ( XZSTC_WARNING, "XZEH02_Lecture_Horloge : SORTIE FCT OK : %s ", pa_Horodate );
                      XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEH02_Lecture_Horloge %s", pa_Horodate );
                      return ( XDC_OK );
                   }
               }
            }
         }
      }
   }

   sem_post(&vg_semaphore); 
   XZST_03EcritureTrace ( XZSTC_WARNING, "Echec Lecture Horloge sur probleme select" );
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEH02_Lecture_Horloge avec NOK " );
   return ( XDC_NOK );

}	


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fermeture la ligne serie connectee au Centrale horaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEH03_Fermeture_Horloge ( void )

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
* XDC_OK 		Succ�s
*
* CONDITION D'UTILISATION
*   Est appelee un fois, des qu'il n'est plus necessaire de lire l'horodate
*
* FONCTION 
*
*   Fermeture de la liaison serie par appel a la primitive XZSS09, en specifiant
*   le descripteur de fichier XZEHV_FD_HORLOGE
------------------------------------------------------*/
{
  static char *version = "$Id: xzeh.c,v 1.10 2017/02/17 13:34:32 gesconf Exp $ : XZEH03_Fermeture_Horloge" ;
  
  XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZEH03_Fermeture_Horloge"  );
  XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEH03_Fermeture_Horloge"  );

  return ( XZSS_09FermetureRS232 ( XZEHV_FD_HORLOGE ) );

}	
