/*E*/
/*Fichier : $Id: esx0_cyc.c,v 1.12 2007/11/20 08:35:06 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2007/11/20 08:35:06 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESX0 * FICHIER esx0_cyc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de gestion cyclique de la tache TESX0, protocole
*   de la tache TESAT.
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1     22 Dec 1994	: Creation
* Mismer.D	version 1.2	26 Jan 1995	:
* Volcic.F	version 1.3	13 Fev 1995	:
* Volcic.F	version 1.4	15 Mar 1995	: Modif Diverses
* Volcic.F	version 1.5	03 Avr 1995	: 
* Volcic.F	version 1.6	04 Avr 1995	:
* Fontaine.C	version 1.7	23 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.8	09 Jun 1995	: Copie fichier HORODATE dans /tmp
* Volcic.F	version 1.9	06 Oct 1995	: Suppr. commentaire sur env_alarme (XDC_EQT_SAT)
* Mismer.D	version 1.10	26 Jan 1995	: Modif niveau trace lecture fichier reception
* Mismer.D      Version 1.11    15 Nov 1996     : Modif Alarmes (DEM/1232)
* JMG   13/11/07        : ajout site de gestion DEM715
*
------------------------------------------------------*/

/* fichiers inclus */

#include "esx0_cyc.h"
#include "ex_mala.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: esx0_cyc.c,v 1.12 2007/11/20 08:35:06 pc2dpdy Exp $ : esx0_cyc" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void Cree_Fichier_Horodatage ( void )

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   Place le fichier HORODATAGE dans le repertoire SATIR/EMISSION
*
------------------------------------------------------*/
{
  static char *version = "$Id: esx0_cyc.c,v 1.12 2007/11/20 08:35:06 pc2dpdy Exp $ : Cree_Fichier_Horodatage" ;

XDY_Horodate	vl_Horodate;
XZSMT_Horodate  vl_Horodatedecompo;
int 		vl_jourSemaine;
int		vl_Fd;
char		pl_PathEtFic[XDC_PATH_ABS_SIZE];
char    	pl_CmdSysteme[XDC_LINE_CMD_SIZE];
XDY_Texte       pl_Lign1;

  /*B Recherche de  l'horodate en sec */
  XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Horodate ) ;
  /*B Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
  XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
             
  /* passage de l'annee sur 2 digit */
  memcpy(&vl_Horodatedecompo[6],&vl_Horodatedecompo[8],12);
  vl_Horodatedecompo[14] = '\0';

             
  XZST_03EcritureTrace( XZSTC_DEBUG1, "trame de date:%s", vl_Horodatedecompo );
  	     
  /*B Creation du fichier de mise a l'heure sur le repertoire EMISSION */
  sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_SATIR_EMISSION, XZEXC_NOM_FICHIER_HORODATAGE );

  /*B Si fichier present le detruire */
  if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) > 0 ) 
  {
    XZSS_13FermerFichier ( vl_Fd );
    XZST_03EcritureTrace(XZSTC_INFO,"Fichier HORODATAGE toujours present ");
    sprintf( pl_CmdSysteme, "afp_rm %s", pl_PathEtFic );
    system ( pl_CmdSysteme );
  }

  /*B Ouverture du fichier d'HORODATAGE en cr≈ation */
  sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_SATIR_TEMP_EMISSION, XZEXC_NOM_FICHIER_HORODATAGE );
  if ( XZSS_12OuvrirFichier ( XZSSC_FIC_CR | XZSSC_FIC_RDWR, pl_PathEtFic, &vl_Fd ) == XZSSC_FIC_INV )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"Impossible creer fichier HORODATAGE");
    XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : Cree_Fichier_Horodatage");
    return;
  }

  /* Remplissage du fichier HORODATAGE */
  sprintf ( pl_Lign1, "%s\n", vl_Horodatedecompo );

  if ( write ( vl_Fd, pl_Lign1, strlen ( pl_Lign1 ) ) < 0 )
  {
    XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'ecrire la 1ere ligne dans le fichier" );
  }
  XZSS_13FermerFichier ( vl_Fd );
  sprintf( pl_CmdSysteme, "chmod +rw %s", pl_PathEtFic );
  system ( pl_CmdSysteme );
  sprintf( pl_CmdSysteme, "cp %s/%s %s", 
	   XZEXC_PATH_SATIR_TEMP_EMISSION, XZEXC_NOM_FICHIER_HORODATAGE, "/tmp" );
  system ( pl_CmdSysteme );
  sprintf( pl_CmdSysteme, "mv %s/%s %s/", 
	   XZEXC_PATH_SATIR_TEMP_EMISSION, XZEXC_NOM_FICHIER_HORODATAGE, XZEXC_PATH_SATIR_EMISSION );
  system ( pl_CmdSysteme );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ey_traitement_cyclique ( int va_NumSockRecep )

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
   static char *version = "$Id: esx0_cyc.c,v 1.12 2007/11/20 08:35:06 pc2dpdy Exp $ : ey_traitement_cyclique" ;

   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg, pl_MsgTimer;
   XDY_Horodate		vl_Horodate;
   XZSMT_Horodate       vl_Horodatedecompo;
   int			vl_Delai;
   XZEXT_CLE_PROG	pl_CleProg;
   int			vl_Fd;
   char			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   struct stat		st_stat,*pl_stat;
   char			pl_NomFichier[XDC_PATH_ABS_SIZE];
   char			pl_CmdSysteme[200];
   XDY_Texte       	pl_FinBloc;
   XDY_Texte       	pl_Lign1;
   XDY_Texte 		pl_LigneFich;
   XDY_Texte 		pl_MsgSocket;
   static int		vg_AlarmeSurveillanceSATIR=0;
   int			vl_cr;


   /*A
    *  Lecture du message recu dans la socket appelante
    */
	
   pl_stat = &st_stat;

   if ( ex_lire_msg ( va_NumSockRecep, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {

      /* Si le message est correct et si ce n'est pas une deconnexion */
      if ( ( vl_LgMsg != 0 ) && ( vl_IndCnx != XZEXC_DECONNEC ) )
      {

  	  /*A Si il s'agit d'un reveil cyclique */
  	  if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
  	  {
  	     sscanf ( &pl_Msg[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
  	  					         &vl_Horodate,
  	  					         &vl_Delai,
  	  					         pl_CleProg );

  	     switch ( vl_Delai )
  	     {
  	         /*A Si il s'agit d'un reveil cyclique 10s */
  	         case ESXC_REVEIL_10S :
  	         {
  	           /* Trace du reveil */
  	           XZST_03EcritureTrace( XZSTC_DEBUG1, "%s", pl_CleProg );
                   
                   /*A Verifier que le fichier HORODATAGE n'est plus pr≈sent sinon le recreer */
		   Cree_Fichier_Horodatage ();

                   /*A lecture des fichiers du repertoire de reception */

		   /*A Faire une liste des fichiers du repertoire SATIR/RECEPTION redirige dans un fichier */
		   /*B Mettre un nom de fichier par ligne */

		   sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_SATIR_TEMP_RECEPTION, XZEXC_NOM_FICHIER_LISTE );
		   sprintf( pl_CmdSysteme, "ls -1 -t -c -r %s > %s", XZEXC_PATH_SATIR_RECEPTION, pl_PathEtFic );
		   system ( pl_CmdSysteme );
 
                   /*A Ouvrir le fichier resultat de la liste du repertoire SATIR/RECEPTION */
  
		   stat ( pl_PathEtFic, pl_stat );
		   /*A Si le fichier n'est pas vide ALORS */
		   if ( pl_stat->st_size != 0 )
		   {
		     if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) > 0 )
		     {
  		       do
		       {

		         /*A Le fichier le plus ancien est en debut de liste */
		         if ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof( pl_LigneFich )) != XZEXC_EOF )
		         {
		           strcpy ( pl_NomFichier, pl_LigneFich );

	                   if ( memcmp ( pl_NomFichier, XZEXC_FIC_TRAVAUX, strlen (XZEXC_FIC_TRAVAUX) ) == 0 )
		           {

                             XZST_03EcritureTrace( XZSTC_WARNING,
			       "fichier %s dans repertoire RECEPTION ",pl_NomFichier );

		             sprintf ( pl_CmdSysteme, "%s/%s",XZEXC_PATH_SATIR_RECEPTION, pl_NomFichier);
			     stat (  pl_CmdSysteme, pl_stat);

		             /*A Si le fichier n'est pas vide et fichier travaux prevu ALORS on le traite */
	                     if ( ( pl_NomFichier[3] == 'p' || pl_NomFichier[3] == 'P' ) && ( pl_stat->st_size != 0 ) )
			     { 
		   
		               sprintf ( pl_CmdSysteme, "cp %s/%s %s/",
		  	  	       XZEXC_PATH_SATIR_RECEPTION, pl_NomFichier, XZEXC_PATH_SATIR_TEMP_RECEPTION );
                               system ( pl_CmdSysteme );

			       sprintf ( pl_CmdSysteme, "afp_rm %s/%s",
					 XZEXC_PATH_SATIR_RECEPTION, pl_NomFichier );
			       
			       system ( pl_CmdSysteme );
			       
			       sprintf ( pl_MsgSocket, "%s $$$ %s %s", 
				       XZEXC_FIC_TRAVAUX, pl_NomFichier, XZEXC_FIN_MSG_SOCK );

			       ex_env_msg( XZEXC_ADR_SOCK_SATSX0, &vg_SockMask, pl_MsgSocket, &vl_cr);
                             }
			     else
			     {  /*B fichier travaux en cours */
			       /* suppression systematique */
		               sprintf ( pl_CmdSysteme, "afp_rm %s/%s",
			  	     XZEXC_PATH_SATIR_RECEPTION, pl_NomFichier );
                               system ( pl_CmdSysteme );
			     }
		           }  
		           else
		           {
		             if ( memcmp ( pl_NomFichier, XZEXC_FIC_ETAT_LIAISON, strlen (XZEXC_FIC_ETAT_LIAISON) ) == 0 )
		             {

		               sprintf ( pl_CmdSysteme, "cp %s/%s %s/",
		    		   XZEXC_PATH_SATIR_RECEPTION, pl_NomFichier, XZEXC_PATH_SATIR_TEMP_RECEPTION );
                               system ( pl_CmdSysteme );

			       sprintf ( pl_CmdSysteme, "afp_rm %s/%s",
					XZEXC_PATH_SATIR_RECEPTION, pl_NomFichier );
				
				system ( pl_CmdSysteme );

			       sprintf ( pl_MsgSocket, 
			  	   "%s $$$ %s %s", 
				   XZEXC_FIC_ETAT_LIAISON, pl_NomFichier, XZEXC_FIN_MSG_SOCK );

			       ex_env_msg( XZEXC_ADR_SOCK_SATSX0, &vg_SockMask, pl_MsgSocket, &vl_cr);
		             }
			     else
			     {
		               /* suppression systematique du fichier non traite */

		               if ( memcmp ( pl_NomFichier, "afp_resource", strlen ("afp_resource") ) != 0 )
			       {
		                 sprintf ( pl_CmdSysteme, "afp_rm %s/%s", XZEXC_PATH_SATIR_RECEPTION, pl_NomFichier );
                                 system ( pl_CmdSysteme );
			       }
		  	     }
		           } /* fin else fichier travaux */
		         } /* plus de nom de fichier dans ficher liste */
			 else
			 {
			   break;
                         }
		       }  while ( pl_stat->st_size != 0 );

                      /*A fermeture du fichier liste */
                       XZSS_13FermerFichier ( vl_Fd );

		     } /* fin ouverture fichier */
		     else
		     {

                       XZST_03EcritureTrace( XZSTC_WARNING,
			       "Erreur ouverture fichier lecture repertoire RECEPTION " );
		     }

                   } /* fin liste non nul */

                 
		   /*A Fin du traitement */
		   sprintf( pl_CmdSysteme, "rm %s/%s", XZEXC_PATH_SATIR_TEMP_RECEPTION, XZEXC_NOM_FICHIER_LISTE );
		   system ( pl_CmdSysteme );


	           break;
  	         }

  	         /*A Si il s'agit d'un reveil cyclique 30s */
  	         case ESXC_REVEIL_30S :
  	         {
  	            /* Trace du reveil */
  	            XZST_03EcritureTrace( XZSTC_DEBUG1, "%s", pl_CleProg );

		    /* Construire le nom du fichier SURVEILLANCE */
		    sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_SATIR_EMISSION, XZEXC_NOM_FICHIER_SURVEILLANCE );

		    if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) > 0 )
		    {
                      XZST_03EcritureTrace ( XZSTC_INFO, "le fichier ETAT_SAE existe" );

                      /*A Si le fichier est present et alarme non presente */
		      if ( vg_AlarmeSurveillanceSATIR == 0 )
		      {
		        /*A Generer une alarme apparition */
		        vg_AlarmeSurveillanceSATIR = 1;
		        		        
                        /*B Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
                        XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
		        
                        XZST_03EcritureTrace ( XZSTC_WARNING, "Alarme -> Fichier surveillance non lu par SATIR" );

		        /*A Envoi de l'alarme */
                        ex_env_alarme  (
                       	vg_NomMachine,
			vl_Horodate,
			XDC_EQT_SAT,
			1,
			XZAMC_DEF_CYCL_SATIR,
			XDC_VRAI,
			NULL,
			XZSC_22NumeroSiteLocal());
		      }

                      XZSS_13FermerFichier ( vl_Fd );
                    }
		    else 
		    {
                      XZST_03EcritureTrace ( XZSTC_INFO, "le fichier ETAT_SAE n'existe pas -> creation" );

                      /*A Si le fichier est absent et alarme presente */
		      if ( vg_AlarmeSurveillanceSATIR == 1 )
		      {
		        /*A Generer un alarme disparition */
		        vg_AlarmeSurveillanceSATIR = 0;
		        		        		        
                        /*B Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
                        XZSM_11ConversionHorodate ( vl_Horodate, &vl_Horodatedecompo );
		        
                        XZST_03EcritureTrace ( XZSTC_WARNING, "Fin Alarme -> Fichier surveillance lu par SATIR" );

		        /*A Envoi de l'alarme */
                        ex_env_alarme  (
                       	vg_NomMachine,
			vl_Horodate,
			XDC_EQT_SAT,
			1,
			XZAMC_DEF_CYCL_SATIR,
			XDC_FAUX,
			NULL,XZSC_22NumeroSiteLocal() );
		      }

		      /*A Creer le fichier SURVEILLANCE */
                      /*B Ouverture du fichier de SURVEILLANCE en cr≈ation */
		      sprintf ( pl_PathEtFic, "%s/%s", 
				XZEXC_PATH_SATIR_TEMP_EMISSION, XZEXC_NOM_FICHIER_SURVEILLANCE );
                      if ( XZSS_12OuvrirFichier ( XZSSC_FIC_CR | XZSSC_FIC_RDWR, pl_PathEtFic, &vl_Fd ) == 
												   XZSSC_FIC_INV )
                      {
                        XZST_03EcritureTrace(XZSTC_WARNING,"Impossible creer fichier SURVEILLANCE");
                        break;
                      }
		      sprintf ( pl_Lign1, "%s\n", "Surveillance SATIR" );
                      sprintf ( pl_FinBloc, "%s\n", "$- Fin Bloc -$" );
                      if ( write ( vl_Fd, pl_Lign1, strlen ( pl_Lign1 ) ) < 0 )
                      {
                        XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'ecrire la 1ere ligne dans le fichier" );
                      }

                      if ( write ( vl_Fd, pl_FinBloc, strlen ( pl_FinBloc ) ) < 0 )
                      {
                        XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'ecrire la fin de bloc dans le fichier" );
                      }
                      XZSS_13FermerFichier ( vl_Fd );
                      sprintf( pl_CmdSysteme, "chmod +rw %s", pl_PathEtFic );
                      system ( pl_CmdSysteme );
                      sprintf( pl_CmdSysteme, "mv %s/%s %s/", 
	                       XZEXC_PATH_SATIR_TEMP_EMISSION, XZEXC_NOM_FICHIER_SURVEILLANCE, 
			       XZEXC_PATH_SATIR_EMISSION );
                      system ( pl_CmdSysteme );
		    }
		    break;
  	         }
  	     }
	  }


	  /*A Si il s'agit d'un reveil pour synchro de la date */
	  if ( strstr ( pl_CleProg, "SX_Timer_Synchro" ) != NULL )
	  {
	     Cree_Fichier_Horodatage ();

  	     /* Rearmement du reveil synchro date */
             ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "SX_Timer_Synchro", va_NumSockRecep, pl_MsgTimer )
  	     
	  }
       }
    }
}


