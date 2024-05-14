/*E*/
/*Fichier : $Id: esx0_tra.c,v 1.8 1996/01/29 16:53:41 volcic Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/01/29 16:53:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESX0 * FICHIER esx0_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de traitement des trames pour la tache TESX0,
*  protocole de la tache TESAT.
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1	27 Dec 1994	: Creation
* Mismer.D	version 1.2	26 Jan 1995	:
* Volcic.F	version 1.3	13 Fev 1995	:
* Volcic.F	version 1.4	15 Mar 1995	: Modif Diverses
* Fontaine.C	version 1.5	23 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.6	02 Jun 1995	: Modif deconnection tesat
* Volcic.F	version 1.7	06 Oct 1995	: Modif gestion fichiers sur volume monte
* Volcic.F	version 1.8	29 Dec 1995	: Modif close fichier si present dans emission
*
------------------------------------------------------*/

/* fichiers inclus */

#include "esx0_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: esx0_tra.c,v 1.8 1996/01/29 16:53:41 volcic Exp $ : esx0_tra";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Decodage de la trame et traitement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*   
* va_socket:socket de lecture
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
* execute sur reception de message socket
*
* FONCTION 
*
*  Decodage de la trame et traitement
------------------------------------------------------*/
{
static char *version = "$Id: esx0_tra.c,v 1.8 1996/01/29 16:53:41 volcic Exp $ : em_traitement_trame";

int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
char			*pl_trame;
XZEXT_MSG_SOCKET	vl_Msg;
XDY_Texte		pl_NomFichier;
XDY_Texte		pl_TypeFichier;
XDY_Texte		pl_Separateur;
char			pl_PathEtFic[XDC_PATH_ABS_SIZE];
char			pl_PathEtFic1[XDC_PATH_ABS_SIZE];
int			vl_Fd;
int			vl_Fd1;
XDY_Texte       	pl_LigneFich;
int			vl_NumLigne;

char	pl_CmdSysteme[XDC_LINE_CMD_SIZE];

XDY_Texte	vl_EtatTravaux;
XDY_Texte	vl_TypFic;

int	vl_Dest1;
int	vl_Dest2;
int	vl_Dest3;
int	vl_Dest4;
int	vl_Dest5;
int	vl_Dest6;
int	vl_Dest7;
int	vl_Dest8;
 

   /*A Gestion des trames provenant de tesat */

   if ( ex_lire_msg ( va_socket, vl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }
   else
   {

      /*A Test d'une eventuelle deconnection avec tesat.x */
      if ( vl_IndCnx == XZEXC_DECONNEC )
      	 ec_sortir ();
      	 
      /*A Scan du message provenant de la socket */
      sscanf(vl_Msg,"%s %s %s ",pl_TypeFichier, pl_Separateur, pl_NomFichier );
      					 

      /* A Construction du nom du fichier */

      sprintf ( pl_PathEtFic, 
	        "%s/%s", XZEXC_PATH_SATIR_TEMP_EMISSION, pl_NomFichier );

      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
        return;
      }

      ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof( pl_LigneFich ) );

      sscanf ( pl_LigneFich,
               "%s ##$ %2d %2d %2d %2d %2d %2d %2d %2d$",
	       vl_TypFic,
	       &vl_Dest1,&vl_Dest2,&vl_Dest3,&vl_Dest4,
	       &vl_Dest5,&vl_Dest6,&vl_Dest7,&vl_Dest8 );

      XZSS_13FermerFichier ( vl_Fd );

      if ( memcmp (pl_TypeFichier,XZEXC_FIC_ACCIDENT, strlen(XZEXC_FIC_ACCIDENT)  ) == 0 ||
           memcmp (pl_TypeFichier,XZEXC_FIC_TRAFIC,   strlen(XZEXC_FIC_TRAFIC)    ) == 0 ||
           memcmp (pl_TypeFichier,XZEXC_FIC_METEO,    strlen(XZEXC_FIC_METEO)     ) == 0 ||
           memcmp (pl_TypeFichier,XZEXC_FIC_FERMETURE,strlen(XZEXC_FIC_FERMETURE) ) == 0 )
      {


        /* A Construction du nom du fichier du repertoire emission */

        sprintf ( pl_PathEtFic, 
	          "%s/%s", XZEXC_PATH_SATIR_EMISSION, pl_NomFichier );

        if ( access (pl_PathEtFic, F_OK) )
        {
	  /* B Le fichier n'existe pas transfert direct */

          sprintf ( pl_CmdSysteme, 
		    "mv %s/%s %s/", XZEXC_PATH_SATIR_TEMP_EMISSION, pl_NomFichier,
		                    XZEXC_PATH_SATIR_EMISSION );
          system ( pl_CmdSysteme );
        }
        else
        {

	  /* B Le fichier existe ajout du nouveau a la suite du precedent */

	  if ( memcmp ( vl_TypFic, "S", 1 ) == 0 || memcmp ( vl_TypFic, "D", 1 ) == 0 ) 
	  {

	    /* Sauvegarde du fichier precedent */ 
            sprintf ( pl_CmdSysteme, "cp %s/%s %s/Sauvegarde", XZEXC_PATH_SATIR_EMISSION, pl_NomFichier, XZEXC_PATH_SATIR_TEMP_EMISSION );
            system ( pl_CmdSysteme );

            sprintf ( pl_CmdSysteme, "afp_rm %s/%s", XZEXC_PATH_SATIR_EMISSION, pl_NomFichier );
            system ( pl_CmdSysteme );

	    /* Concatenation des fichiers et remise en place */
            sprintf ( pl_CmdSysteme, "cat %s/Sauvegarde %s/%s > %s/%s", XZEXC_PATH_SATIR_TEMP_EMISSION, XZEXC_PATH_SATIR_TEMP_EMISSION, 
            								pl_NomFichier, XZEXC_PATH_SATIR_EMISSION, pl_NomFichier );
            system ( pl_CmdSysteme );
          }
	  else 
	  {
	    /* Sauvegarde du fichier precedent */ 
            sprintf ( pl_CmdSysteme, "cp %s/%s %s/Sauvegarde", XZEXC_PATH_SATIR_EMISSION, pl_NomFichier, XZEXC_PATH_SATIR_TEMP_EMISSION );
            system ( pl_CmdSysteme );

            sprintf ( pl_CmdSysteme, "afp_rm %s/%s", XZEXC_PATH_SATIR_EMISSION, pl_NomFichier );
            system ( pl_CmdSysteme );

            /* Modification de la ligne d'entete du fichier Sauvegarde */

	    sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_SATIR_TEMP_EMISSION, "Sauvegarde" );

	    sprintf ( pl_PathEtFic1, "%s/%s", XZEXC_PATH_SATIR_TEMP_EMISSION, "Sauvegarde1" );

	    vl_NumLigne = 1;
            if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) > 0 )
	    {
              if ( XZSS_12OuvrirFichier ( XZSSC_FIC_CR | XZSSC_FIC_RDWR, pl_PathEtFic1, &vl_Fd1 ) == XDC_OK )
	      {
	        while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof( pl_LigneFich )) != XZEXC_EOF )
	        {
		  if ( vl_NumLigne == 1 ) 
		  {
		    pl_LigneFich[0] = 'F';
		  }
		  write ( vl_Fd1, pl_LigneFich, strlen (pl_LigneFich) );
		  vl_NumLigne ++;
	        }
                XZSS_13FermerFichier ( vl_Fd1 );
	      }
              XZSS_13FermerFichier ( vl_Fd );
	    }
                 

	    /* Concatenation des fichiers et remise en place */
            sprintf ( pl_CmdSysteme, "cat %s/Sauvegarde1 %s/%s > %s/%s", XZEXC_PATH_SATIR_TEMP_EMISSION, XZEXC_PATH_SATIR_TEMP_EMISSION, 
            								pl_NomFichier, XZEXC_PATH_SATIR_EMISSION, pl_NomFichier );
            system ( pl_CmdSysteme );
	  }
	  /* Suppression des fichiers intermediaires */
          sprintf ( pl_CmdSysteme, "rm %s/Sauvegarde", XZEXC_PATH_SATIR_TEMP_EMISSION);
          system ( pl_CmdSysteme );

          sprintf ( pl_CmdSysteme, "rm %s/Sauvegarde1", XZEXC_PATH_SATIR_TEMP_EMISSION);
          system ( pl_CmdSysteme );

          sprintf ( pl_CmdSysteme, "rm %s/%s", XZEXC_PATH_SATIR_TEMP_EMISSION, pl_NomFichier );
          system ( pl_CmdSysteme );
        }
      }

      if ( memcmp(pl_TypeFichier,XZEXC_FIC_TRAVAUX,strlen(XZEXC_FIC_TRAVAUX) ) == 0 ||
           memcmp(pl_TypeFichier,XZEXC_FIC_INFO_USAGER,strlen(XZEXC_FIC_INFO_USAGER) ) == 0 )
      {
	/* Le fichier est ecrase si il existe */

        sprintf ( pl_CmdSysteme, "mv %s/%s %s/", XZEXC_PATH_SATIR_TEMP_EMISSION, pl_NomFichier, XZEXC_PATH_SATIR_EMISSION );
        system ( pl_CmdSysteme );
      }
   }
}

