/*E*/
/*Fichier : $Id: etmi_uti.c,v 1.12 1997/06/06 10:03:14 mismer Exp $ 	Release : $Revision: 1.12 $ 		Date : $Date: 1997/06/06 10:03:14 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TETMi  * FICHIER etmi_uti.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition fonctions communes modules TETMi
* 
*
------------------------------------------------------
* HISTORIQUE :
*
*  Nouaille M.	Version 1.1	02 Dec 1994		: Creation
*  Nouaille M.  Version 1.2     24 Jan 1995             : RAS
*  Fontaine C.  Version 1.3     18 Avr 1995             : RAS
*  Fontaine C.  Version 1.4     19 Avr 1995             : Prise en compte Vitesse Repondeur
*  Fontaine C.  Version 1.5     03 Mai 1995             : Reprise entete et commentaire 
*  Fontaine C.  Version 1.6     17 Mai 1995             : Reprise historique
*  Mismer.D  	version 1.7	05 Oct 1995		: Modif niveau trace
*  Mismer.D  	version 1.8	20 Oct 1995		: Modif time_out repondeur
*  Mismer.D  	version 1.9	07 Dec 1995		: Modif ouverture connexion transerver
*  Mismer.D  	version 1.10	14 Dec 1995		: Remodif ouverture connexion transerver
*  Mismer.D  	version 1.11	18 Dec 1995		: Reremodif ouverture connexion transerver
*  Mismer.D     version 1.12    30 Mai 1997     	: Mise en place serveur de terminaux XYPLEX (DEM/1431)
*
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "etmi_uti.h"       

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: etmi_uti.c,v 1.12 1997/06/06 10:03:14 mismer Exp $ etmi_uti" ;

/* definition de fonctions internes */

int ecom_lire_config_MAA (int );

/* declaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Initialisation tache TETMi
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int ecom_init_TEL( 	XZEXT_ST_Config *pa_Config, 
*			XZEXT_MASK **pa_SockList, 
*			XZEXT_ST_Sockets *pa_desSocks )
*
*
* ARGUMENTS EN ENTREE :
*   
* pa_Config	: pointeur sur structure stokage config non initialisee.
* pa_SockList	: pointeur sur la liste des sockets.
* pa_desSocks	: pointeur sur les sockets specifiques taches TETxi.
*
* ARGUMENTS EN SORTIE :
* pa_Config	: pointeur sur structure config mise a jour
* pa_SockList	: pointeur sur la liste des sockets mise a jour
* pa_desSocks	: pointeur sur les sockets specifiques taches TETximises a jour.
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
*   lecture fichier config. lignes pour recuperer ds. pa_ConfigLigne la config de la ligne geree 
*   connexion en mode client sur la socket serveur des terminaux.
*   
------------------------------------------------------*/
int ecom_init_TEL( XZEXT_ST_Config *pa_Config, XZEXT_MASK **pa_SockList, XZEXT_ST_Sockets *pa_DesSocks )
{
    static char *version = "%W %G : ecom_init_TEL" ;
    
    XZEXT_MASK 	*vl_MasqueST = NULL;		/* masque socket config. ligne geree */
    int		vl_DesSock;			/* socket comms. avec le ST par la console */
    char	vl_CmdeST[512];			/* commande pour le ST */
    int         vl_NbBits;                      /* nombre de bits pour config Liaison Serie */    
    char        vl_Speed[5];                    /* vitesse de transmission */
    static int	vl_CptEssai  = 0;
    int 	vl_CodeRet   = 0;
    
     XZST_03EcritureTrace( XZSTC_FONCTION, "IN  : ecom_init_TEL " );
    
    /*A
     * lecture config. ligne serie geree.
     * remplissage donnees config. ds. la structure pa_ConfigLigne.
     */
     
    /*A SI lecture config. echoue, retour XDC_NOK */
    if( ecom_lect_config_ligne_serie( pa_Config->nom_machine, 
    					  &(pa_Config->ligne_serie), 
    					  pa_Config->no_ligne )  != XDC_OK )
    {
    	return( XDC_NOK );
    }
    /*A recuperation no port domaine internet */
    pa_Config->no_port = 2000 + (pa_Config->no_ligne * 100);
    /*A SI desc. socket lserie diff. de XZEXC_SOCKET_NULL ALORS fermeture socket */
    if( pa_DesSocks->lser != XZEXC_SOCKET_NULL )
    {
       ex_fin_cnx( pa_DesSocks->lser, pa_SockList );
       pa_DesSocks->lser = XZEXC_SOCKET_NULL;
    }
    
    /*A configuration de la BAUD RATE de la ligne geree */
    /*A SI ouverture console ST echoue */
    while  ( 1 )
    {
       if( (vl_CodeRet = ex_ouvrir_console_ST(pa_Config->ligne_serie.NumeroST, &vl_DesSock, &vl_MasqueST)) == XDC_OK )
          break;
       else
       {
    	  XZST_03EcritureTrace( XZSTC_WARNING, "ecom_init_TEL : Echec %deme essai connexion avec %s", vl_CptEssai, pa_Config->ligne_serie.NumeroST );
          if ( vl_CptEssai++ > 1 ) break;
          sleep ( 10 );
       }
    }
    if ( vl_CodeRet == XDC_OK )
    {
       vl_CptEssai = 0;
       /*A formatage commande fermeture session transerver  */
       sprintf( vl_CmdeST, "KILL PORT %d SESSION ALL\r", pa_Config->no_ligne );
       if( ex_envoyer_commande_ST( vl_CmdeST, vl_DesSock, &vl_MasqueST ) != XDC_OK )
       {
           /*A ALORS trace "ecom_init_TEL : Emission cmde. fermeture session transerver voie%d  impossible !!" */
    	   XZST_03EcritureTrace( XZSTC_WARNING, "ecom_init_TEL : Emission  cmde. fermeture session transerver voie%d  impossible !!!!",
    		(pa_Config->no_ligne) -1 );
       }
       

       /*A
       * connexion soket en mode client pour les comms. avec le serveur des terminaux
       */

       XZST_03EcritureTrace( XZSTC_DEBUG1, "ecom_init_TEL : cnx. client %s, %d ", 
    				pa_Config->ligne_serie.NumeroST, pa_Config->no_port );

       /*B Attribution de la vitesse dans le cas repondeur */
        vl_NbBits = ecom_lire_config_MAA( pa_Config->no_ligne );
        if ( vl_NbBits == 7 )
           strcpy ( vl_Speed , "4800");
        else
	   strcpy ( vl_Speed , pa_Config->ligne_serie.Vitesse );
       
       sleep(1);

       /*A SI connexion echoue */
       if( ex_cnx_client_ST( pa_Config->ligne_serie.NumeroST,
		   	   pa_Config->no_port,
			   pa_SockList, 
			   &(pa_DesSocks->lser) 	) != XDC_OK  )
       {
    	   /*A ALORS */
	   /*B ecriture trace : " Connexion socket ligne serie mode client impossible !" */
	   XZST_03EcritureTrace( XZSTC_WARNING, 
	   	   "ecom_init_TEL TELMi ligne %d : Connexion sur socket ligne serie %d ST <%s port %d> en mode client impossible !",
		    pa_Config->no_ligne, pa_Config->no_ligne, pa_Config->ligne_serie.NumeroST, pa_Config->no_port );
	   /*B mise XZEXC_SOCKET_NULL ds. pa_DesSocks->lser */
	   pa_DesSocks->lser = XZEXC_SOCKET_NULL;
	   
           /*A fermetur conexion avec la console ST */
           ex_fin_cnx( vl_DesSock, &vl_MasqueST );
	   /*B retour ELxiC_ST_NOK */
	   return( ELxiC_ST_NOK );
       }

       /*A formatage commande ST config BAUD RATE : "set port x speed  yyyy\r" */
       sprintf( vl_CmdeST, "SET PO %d SPEED %s CHAR SIZE %d \r\0", pa_Config->no_ligne, vl_Speed , vl_NbBits );
       XZST_03EcritureTrace( XZSTC_WARNING, "Cmde ST BAUD RATE : %s", vl_CmdeST ); 
    
       /*A emission cmde config sur console ST */
       /*A SI emission cmde console ST echoue */
       if( ex_envoyer_commande_ST( vl_CmdeST, vl_DesSock, &vl_MasqueST ) != XDC_OK )
       {
          /*A ALORS trace "ecom_init_TEL TELMi ligne %d : Emission cmde. config. vitesse %s ligne %d sur console ST %s impossible !!" */
           XZST_03EcritureTrace( XZSTC_WARNING, "ecom_init_TEL TELMi ligne %d : Emission cmde. config. vitesse %s ligne %d sur console ST %s impossible !!",
    	   	      pa_Config->no_ligne, pa_Config->ligne_serie.Vitesse, pa_Config->no_ligne, pa_Config->ligne_serie.NumeroST );
           /*A fermetur conexion avec la console ST */
           ex_fin_cnx( vl_DesSock, &vl_MasqueST );
	   /*A retour  ELxiC_ST_NOK */
    	   return( ELxiC_ST_NOK );
       }

       /*A fermetur conexion avec la console ST */
       ex_fin_cnx( vl_DesSock, &vl_MasqueST );
    }
    else
    {
    	/*A ALORS trace "Ouverture console ST impossible !!" */
    	XZST_03EcritureTrace( XZSTC_WARNING, "ecom_init_TEL TELMi ligne %d : Ouverture console ST %s pour config. ligne serie %d impossible",
    		pa_Config->no_ligne, pa_Config->ligne_serie.NumeroST, pa_Config->no_ligne);
        return( ELxiC_ST_NOK );  
    }
        
    /*A retour XDC_OK */
    XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : ecom_init_TEL ");
    return( XDC_OK );
    
}



/*X*/
/*---------------------------------------------------
* SERVICE RENDU :
*  Cette fonction effectue la lecture du fichier Config_AUTOTEL_<Nom_Machine>
*  et retourne le nombre de bits pour la config de la liaison serie :
*          7 bits pour la LS repondeur,
*          8 bits pour la LS MAA.
*
----------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ecom_lire_config_MAA (int va_NumLS )

/*
* ARGUMENTS EN ENTREE :
*   va_NumLS            : Numero de la LS
*
* ARGUMENTS EN SORTIE :
*   Nombre de bits pour la config :
*      7 pour la LS repondeur,
*      8 pour la LS MAA.
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
*
----------------------------------------------------*/
{
  static char *version = "$Id: etmi_uti.c,v 1.12 1997/06/06 10:03:14 mismer Exp $ : ecom_lire_config_MAA" ;

  int				vl_Fd;
  char  			pl_PathEtFic[XDC_PATH_ABS_SIZE];
  XDY_Texte			pl_LigneFich;
  ETEL_DONNEES_MAA		vl_DonneesMAA = {0};
  int				vl_NbParam  = 0;
  int				vl_int1, vl_int2, vl_int3 ;


      XZST_03EcritureTrace( XZSTC_WARNING, " Entree dans fonction ecom_lire_config_MAA ");

      /*A Ouverture du fichier de configuration des MAA */
      /* Construction du nom du fichier XDF_Config_AUTOTEL_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_AUTOTEL,vg_Config.nom_machine );

      /* Ouverture du fichier XDF_Config_AUTOTEL_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
	/* Ecriture trace absence fichier */
	XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic );
	/* Sortie de la fonction */
        return ( 0 );
      }
      
      /*A Lecture de chaque ligne du fichiers */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF )
      {

	/* Lire Ligne suivante si c'est une ligne de commentaire */
	if ( *pl_LigneFich != XDF_COMMENTAIRE )
	{
	  /*B Extraire les parametres du MAA */
	  vl_NbParam = sscanf ( pl_LigneFich, "%d %s %d %s %d %s",
				   &vl_int1,
				   vl_DonneesMAA.Config.NumeroST,
				   &vl_int2,
				   vl_DonneesMAA.Config.AdrAUTOTEL,
                                   &vl_int3,
				   vl_DonneesMAA.Config.RepAssoc);

	  /*B Si tout les parametres ont ete lus et que vl_int2 = va_NumLS */
	  if ( vl_NbParam == 6 )
	  {
	    if (vl_int2 == va_NumLS )
	    {
	      /* Si RepAssoc = 1 cas du repondeur --> retour de 7 sinon retour de 8 */
	      XZST_03EcritureTrace( XZSTC_WARNING, " ecom_lire_Config_MAA : NumLS trouve ");

              if ( atoi( vl_DonneesMAA.Config.RepAssoc) == XDC_VRAI )
	      {
   	        XZST_03EcritureTrace( XZSTC_WARNING, " ecom_lire_Config_MAA : config 7 bits ");

		return (7);
              }
	      else
	      {
		XZST_03EcritureTrace( XZSTC_WARNING, " ecom_lire_Config_MAA : config 8 bits " );

		return (8);
              }
            }
          }
	}									      
      }
      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );

      return(0);

}      



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Extraction d'une demande TEL d'un msg. socket ds. une structure cmde. TEL
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int	ecom_extraire_cmd_TEL( XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_LCR *pa_CmdeLCR,
*				 int va_DesSockClnt)
*
*
* ARGUMENTS EN ENTREE :
*    pa_MsgSock	: pointeur sur le message contenant la demande LCR d'un tache TEzzz.
*    va_DesSockClnt : desc. socket tache cliente TEzzz ayant fait la demande LCR.
*
* ARGUMENTS EN SORTIE :
*   
*    pa_CmdLCR	: pointeur sur la zone de stokage de la demande LCR depuillee.
*
* CODE RETOUR : 
*   XDC_OK si tout est OK.
*   XDC_NOK Sinon.
* CONDITION D'UTILISATION
*   
*   pa_MsgSock pointe sur un message recu sur une socket donnees contenant une demande LCR.
*
* FONCTION 
*   depouillement de la demande LCR et stokage ds. la structure cmde. LCR.
*   
------------------------------------------------------*/
int	ecom_extraire_cmd_TEL( 	XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_LCR *pa_CmdeLCR, 
				int va_DesSockClnt )
{
    static char *version = "$Id: etmi_uti.c,v 1.12 1997/06/06 10:03:14 mismer Exp $ : ecom_extraire_cmd_TEL" ;
    
    char		*pl_DebTrameLCR;	/* pointe sur le debut de la trame LCR */
    XDY_Texte		vl_Texte;	/* juste pour rec. le separateur de trame */
    

    /*A SI depouillement message socket echoue */
    if( sscanf( &(pa_MsgSock->msg[strlen(XZEXC_DEMANDE_TEL)]), "%d %s %d %d %s",
           			&(pa_CmdeLCR->type_cmd),
           			pa_CmdeLCR->rgs,
 		    		&(pa_CmdeLCR->no_cmd),
		    		&(pa_CmdeLCR->prior),
          			vl_Texte ) != 5 
          	|| (pl_DebTrameLCR = strstr( pa_MsgSock->msg, XZEXC_SEP_TRAME )) == NULL )
    {
    	/*A ALORS trace " Demande LCR incorrecte <message> */
        XZST_03EcritureTrace( XZSTC_WARNING, "ecom_extraire_cmd_TEL: Demande incorrecte :\n <%s>\n", pa_MsgSock->msg );
        /*A retour XDC_NOK */
        return( XDC_NOK );
    }
            
    /*B recuperation trame */
    pl_DebTrameLCR += sizeof(XZEXC_SEP_TRAME);
    pa_MsgSock->msg[pa_MsgSock->len-(strstr(pa_MsgSock->msg, XZEXC_FIN_MSG_SOCK) == NULL ? 0 : sizeof(XZEXC_FIN_MSG_SOCK))] = '\0';
    strcpy( pa_CmdeLCR->cmd_LCR, pl_DebTrameLCR );
 
    XZST_03EcritureTrace( XZSTC_INFO, "ecom_extraire_cmd_TEL: %d %d trame <%s>\n", 
            		sizeof(XZEXC_SEP_TRAME), sizeof(XZEXC_FIN_MSG_SOCK), pa_CmdeLCR->cmd_LCR );
            								 
    /*B attachement desc. socket client a sa demande */
    pa_CmdeLCR->sock_clnt = va_DesSockClnt;
            
    /*B le type du message n'est pas utilise par TETEL */
    pa_CmdeLCR->type_msg = 0;
            
    /*B le champs ack n'est pas utilise par TETEL */
    pa_CmdeLCR->ack[0] = XZEXC_NULC;
    
    /*B si la cmde va generer un echange en blocs, le 1er a recevoir a le no. 0 */
    pa_CmdeLCR->no_blc_suivant = '0';

    /*A retour XDC_OK */  
    return( XDC_OK );

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet de lire un message dans 
*   la socket specifiee. 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ecom_lire_msg_TEL (
 		int		va_sd_sock,
		char 		*pa_message,
 		int		*pa_lg_msg,
 		XZEXT_MASK *  	*pa_masque,
		int		*pa_deconnex,
		double          va_attente )

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donnees.
*   pa_message	:	pointeur de reception du message.
*   pa_masque	:	masque de lecture.
*   va_attente  :       temps d'attente nouveaux caracteres
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque	:	masque de lecture.
*   pa_message	:	pointeur de reception du message.
*   pa_lg_msg	:	pointeur de reception de la longueur du message.
*   pa_deconnex	:	indicateur de deconnexion.
*
* CODE RETOUR : 
*
*   XDC_OK		succes
*   XDC_NOK		lecture message socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Extraire un message d'une socket de donnees.
*   Retourner le message et la longueur du message.
*
------------------------------------------------------*/
{
   static char *version = "$Id: etmi_uti.c,v 1.12 1997/06/06 10:03:14 mismer Exp $ : ecom_lire_msg_TEL ";
   register int			vl_valeurRetour = XDC_OK,
   				vl_NbCar        = 0,
  				vl_cr           = 0;
   XZEXT_MSG_SOCKET		vl_message      = ""; 
   int				vl_index	= 0;
   
   /*A Lecture de la taille du message de la socket specifiee. */
   fcntl ( va_sd_sock, F_SETFL, O_NDELAY | fcntl ( va_sd_sock, F_GETFL, 0 ) );
   sem_wait(&vg_semaphore); 
   if ( ( vl_cr = recv ( va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET), MSG_PEEK ) ) == -1 ) 				  				
   {   
      sem_post(&vg_semaphore); 
      /*B Ecriture dans le fichier trace en cas d'echec */
      XZST_03EcritureTrace( XZSTC_WARNING, "Erreur de lecture de message sur une socket." );
      vl_valeurRetour = XDC_NOK; 
   }
   else
   {      
      sem_post(&vg_semaphore); 
      /*A Si la longueur du message est nulle */
      if ( vl_cr == 0)
      {
         /*A Fermeture de la socket */
        
         sem_wait(&vg_semaphore); 
         read ( va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET) );
         sem_post(&vg_semaphore); 
         ex_fin_cnx ( va_sd_sock, pa_masque );
         
       	 /*A Positionnement a VRAI de l'indicateur de deconnexion socket */
         *pa_deconnex = XZEXC_DECONNEC;
      }
      else
      {
      	 /*A Positionnement a FAUX de l'indicateur de deconnexion socket */
         *pa_deconnex = XZEXC_CONNEC;
         
         /*A Recherche si le message a ete recu en entier */
         while ( (ex_select(va_sd_sock, va_attente ) == XDC_OK) && (vl_NbCar < sizeof(XZEXT_MSG_SOCKET)) ) 
         {
            sem_wait(&vg_semaphore); 
            if ( ( vl_cr = read ( va_sd_sock, vl_message, sizeof(XZEXT_MSG_SOCKET)-2 ) ) == -1 )
            {
               sem_post(&vg_semaphore); 
               XZST_03EcritureTrace( XZSTC_WARNING, "Erreur de lecture de message socket:%d .", va_sd_sock );
               return ( XDC_NOK );
            }
            sem_post(&vg_semaphore); 

            /*A Recopie du message pour l'appelant avec suppression des craracteres non affichables */
            for (vl_index = 0 ; vl_index < vl_cr; vl_index++ )
            {
               if ( ((vl_message[vl_index] >= '0') && (vl_message[vl_index] <= '9')) ||
                    ((vl_message[vl_index] >= 'A') && (vl_message[vl_index] <= 'Z')) ||
                    ((vl_message[vl_index] >= 'a') && (vl_message[vl_index] <= 'z')) ||
                    (vl_message[vl_index] == '*') )
               {
                  pa_message[vl_NbCar++] = vl_message[vl_index];
               }
               else
               {
                  if ( pa_message[(vl_NbCar>0) ? vl_NbCar-1 : 0] != vl_message[vl_index] )
                     pa_message[vl_NbCar++] = vl_message[vl_index];
               }
            }
         }
         
         pa_message[vl_NbCar] = '\0';
         *pa_lg_msg = vl_NbCar;
       }      
   }

   return ( vl_valeurRetour );   

}

