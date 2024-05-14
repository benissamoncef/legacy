/*E*/
/*Fichier : $Id: etmi_dir.c,v 1.11 1996/05/23 17:12:27 volcic Exp $      Release : $Revision: 1.11 $        Date : $Date: 1996/05/23 17:12:27 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TETMi * FICHIER etmi_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Programme principale de la tache TETMi 
* 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Fontaine C.	Version 1.2	08 Mar 1995	: Prise en compte modif. repondeur
* Fontaine C.   Version 1.3     03 Mai 1995     : Reprise des entetes et des commentaires
* D.Mismer      Version 1.4     19 Mai 1995     : Modif gestion deconnexion ST
* Mismer.D 	version 1.5 	02 Jun 1995 	: Modif arg XZST_10ArchiverMsgSyst
* Mismer.D 	version 1.6 	27 Jun 1995 	: Modif arg trace reconnexion ST
* Mismer.D  	version 1.7	31 Aou 1995	: Modif connexion socket RTserver suite
* Mismer.D  	version 1.8	05 Oct 1995	: Correction appel fonction repondeur et niveau trace
* Mismer.D  	version 1.9	20 Oct 1995	: Correction appel fonction repondeur suite
* Mismer.D  	version 1.10	08 Fev 1996	: Ajout test connexion transerver cycliquement
* Mismer.D  	version 1.11	20 Mai 1996	: Ajout renvoi compte rendu apres alarme (DEM/1109)
*
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "etmi_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

/* strucutre donn�es pour le call back en local */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: etmi_dir.c,v 1.11 1996/05/23 17:12:27 volcic Exp $ : etmi_dir" ;

/* declaration de fonctions externes */
extern int ecom_init_TEL( XZEXT_ST_Config *pa_Config, XZEXT_MASK **, XZEXT_ST_Sockets * );
extern int ecom_extraire_cmd_TEL( XZEXT_Msg_Socket *, XZEXT_Cmde_LCR *, int);


/*A
* decl. fonctions controle protocole 
*/
extern void	ecfg_sortir(void) ;
extern void 	epro_envoi_trame_TEL( XZEXT_Cmde_LCR * );
extern void 	epro_envoi_trame_REP( XZEXT_Cmde_LCR * );


/* definitions fonctions internes */

/*A
 * decl. fonctions traitement messages : gestion TEL, INIT, ARRET -> etmi_pro.c
 */

/* definition de fonctions internes */
void edir_traitement_msg_sock( XZEXT_Msg_Socket * );
void edir_traitement_dcnx( void );
void edir_traitement_liste_cmdes_TEL( void );
void edir_traitement_liaison_ST( void );



/*X*/
/* Fichier : $Id: etmi_dir.c,v 1.11 1996/05/23 17:12:27 volcic Exp $      Release : $Revision: 1.11 $        Date : $Date: 1996/05/23 17:12:27 $
------------------------------------------------------
* SERVICE RENDU : 
* Programme principal
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void main( int argc, char **argv )

/*
* ARGUMENTS EN ENTREE :
*   argc : Nombre d'arguments
*   argv : liste des arguments
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
*   Programme principale tache TETMi.
*
------------------------------------------------------*/
{
    static char *version = "$Id: etmi_dir.c,v 1.11 1996/05/23 17:12:27 volcic Exp $ : main" ;

	int	vl_ValRet 	= XDC_OK;		/* valeur retour */
	int	vl_Possible	= XDC_NOK;
	int	vl_EtatCnx 	= XZEXC_CONNEC_UNDEF;	/* etat connexion socket */
	XZEXT_Msg_Socket	vl_MsgSock;		/* struct. creation msg. socket */
	XZEXT_MASK		*pl_Mask;
    	XDY_Texte		vl_Texte;
        XZEXT_ADR_SOCK		pl_SrvTelmi;
        int			vl_result_lect; /* resultat lecture message */        


	/*A
	* sequence initialisation tache TETMi
	*/
	
	/*A init a zero des descs. sockets specifiques taches TETMi */
	vg_DesSocks.curr = XZEXC_SOCKET_NULL;
	vg_DesSocks.ect = XZEXC_SOCKET_NULL;
	vg_DesSocks.lser = XZEXC_SOCKET_NULL;
	vg_DesSocks.rtsrv = XZEXC_SOCKET_NULL;
	
	/*A Recuperation nom machine */
	strcpy( vg_Config.nom_machine, argv[1] );	
	
	/*A Recuperation no. ligne serie */
	vg_Config.no_ligne = atoi( argv[2] );
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"etmi_dir.x : Connexion au RTserver...." );
	
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A SI Connexion au RTserver et initialisation des variables echoue */
	if(  XZSC_01InitCnxSup(  NULL,
				argv[0],
				XZSCC_INTERDIRE,
				XZSCC_AUTORISER,
				XZSCC_INTERDIRE,
				ecfg_sortir,
				"/dev/null" ) != XDC_OK  )
	{
	   /*B ALORS Affichage Trace "Ouverture connexion avec le RTserver impossible!!!" */
	   XZST_03EcritureTrace( XZSTC_FATAL, "Ouverture connexion avec le RTserver impossible!!!" );
	   /*B exit NOK */
	   exit( XDC_NOK );
	}

	XZST_03EcritureTrace( 	XZSTC_DEBUG1, "Ouverture cnx. RTServer reussie ..." );
	
	/*A Creation du repertoire pour les sockets */
	ex_rep_socket ( vg_Config.nom_machine );
	
	/*A
	* connexion soket en mode serveur pour comms. avec la tache traitement
	*/
	/*A SI connexion echoue */
        sprintf ( pl_SrvTelmi, "%s%d", XZEXC_ADR_SOCK_TETM, vg_Config.no_ligne );
	if( ex_cnx_serveur( pl_SrvTelmi, &vg_SockList, &vg_DesSocks.ect ) != XDC_OK )
	{
	    /*A ALORS */
	    /*B ecriture trace : " Connexion socket ecoute mode serveur impossible !" */
	    XZST_03EcritureTrace( XZSTC_FATAL, " Connexion socket ecoute mode serveur impossible !" );
	    /*B sortie XDC_NOK */
	    exit( XDC_NOK );
	}

	XZST_03EcritureTrace( 	XZSTC_DEBUG1, " Nom machine %s, no. ligne serie %d ",
			 	vg_Config.nom_machine, vg_Config.no_ligne ); 

	/*A
	 * Initialisation configuration de la ligne serie ds. vg_Config
	 */
	XZST_03EcritureTrace( XZSTC_INFO, "etmi_dir TETMI ligne %d : initialisation config ...",
			vg_Config.no_ligne );

	if ( (vg_EtatST = ecom_init_TEL( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
	   XZST_03EcritureTrace( XZSTC_WARNING, " Config No port %d impossible",  vg_Config.no_port ); 
	else
	   XZST_03EcritureTrace( XZSTC_WARNING, " Fin init config No port %d ",  vg_Config.no_port ); 
		

	/*A
	* Boucle infinie FAIRE TOUJOURS
	*/
	while( 1 )
	{
	
	    /*A traitement des eventuelles cmdes. TEL empilees ds. la liste de cmdes. non traitees */
	    edir_traitement_liste_cmdes_TEL();
    
	    /*A SI attente message sur liste sockets : ecoute, lserie, donnees client echoue */
	    if( ex_attendre_msg( vg_DesSocks.ect, &vg_SockList, &vg_DesSocks.curr ) != XDC_OK )
	    {
	    	/*A ALORS ecriture trace :  " Probleme attente message sur socket(s)." */
              	XZST_03EcritureTrace( XZSTC_WARNING, " Probleme attente message sur socket(s)." );
                ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, vg_DesSocks.curr, &vg_SockList );
            }
            else
            {
                ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, 0, &vg_SockList );
	    	
	    	/*B SI socket active = socket RTServer */
	    	if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
	    	{
	    	    /*B ALORS appel primitive standard lecture RTWorks */
	    	}
	    	/*B SINON SI evenement sur une socket diff. de la socket d'ecoute 
	     	*  (si. evt. sur socket ecoute <=> cnx. d'un client geree par ex_attendre_msg())
	     	*  tout est O.K. => lecture sur socket active */
	    	else if( vg_DesSocks.curr != XZEXC_NO_EVT 	/* une socket active */
	    		&& vg_DesSocks.curr != vg_DesSocks.ect) /* cas connexion client */
	    	{
	    	    XZST_03EcritureTrace( XZSTC_DEBUG1, "etmi_dir.x : evenement sur une socket diff. de la socket d'ecoute....." );
	    	
	    	    /*B detection adresse socket active */
	    	    ecom_detecte_clnt( &vg_DesSocks, &vg_SockList );
	    	    
	    	    XZST_03EcritureTrace( XZSTC_DEBUG1, "etmi_dir.x : lecture sur socket %d active.", 
	    	    			   vg_DesSocks.curr );
	    	
	    	    /*B SI lecture sur socket active echoue */
	    	    if (edir_detecte_type_sock() == XZEXC_SOCK_ACT_LSER) 
	    	    {
	    	    	  vl_result_lect = ecom_lire_msg_TEL( vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len, &vg_SockList, &vl_EtatCnx, 2.0 ) ;
	    	    }
	    	    else
	    	    {
	    		  vl_result_lect = ex_lire_msg( vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len, &vg_SockList, &vl_EtatCnx ) ;
	    	    }
	    	    
	    	    if ( vl_result_lect != XDC_OK )
	    	    {
	    	    	/*B ALORS ecriture trace :  Probleme lecture socket adresse <vg_DesSocks.curr_addr> */
	    	    	XZST_03EcritureTrace( XZSTC_WARNING, " Probleme lecture socket adresse %s...", vg_DesSocks.curr_addr );
	    	    }

		    /*B SINON */
		    else 
		    {
		    	/*B CAS etat connexion socket active */
		    	switch( vl_EtatCnx )
		    	{
		    	/*B cas : XZEXC_DECONNEC */
		    	case XZEXC_DECONNEC:
		    	    
		    	    XZST_03EcritureTrace( XZSTC_WARNING, "Deconnexion <%s>....", vg_DesSocks.curr_addr );
		    	
		    	    /*B appel fonction traitement dcnxion : edir_traitement_dcnx() */
		    	    edir_traitement_dcnx();
		    	    break;
			
			/*B cas : XZEXC_CONNEC */
		    	case XZEXC_CONNEC :
		    	
		    	    XZST_03EcritureTrace( XZSTC_DEBUG1, "Connexion <%s>...", vg_DesSocks.curr_addr );
		    			
		    	    /*B appel fonction trait. mess. recu edir_traitement_msg_sock( &vl_MsgSock )*/
		    	    edir_traitement_msg_sock( &vl_MsgSock );
		    	    break;
		    	
		    	/*B cas par defaut */
		        default :			
		    	    /*B ecriture trace : Etat connexion socket non defini..." */
		    	    XZST_03EcritureTrace( XZSTC_WARNING, " Etat connexion socket non defini...");
		    	    break;
		    	}
		    	/* FIN CAS( vl_EtatCnx ) */
		    }
		    /* FIN SI( ex_lire_msg()... SINON... */
		}
		/* FIN SI socket server SINON SI( socket active autre que socket ecoute ) */
	    }
	    /* FIN SI( ex_attendre()... SINON.. */
	}
	/* FIN FAIRE TOUJOURS */

}
	




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement du message receptionne par etmi_dir sur la socket active.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void edir_traitement_msg_sock( XZEXT_Msg_Socket *pa_MsgSock )
*
* ARGUMENTS EN ENTREE :
*
* pa_DesSocks	: pointeur sur les descs. sockets specifiques tache TETMi.
* pa_MsgSock	: pointeur sur le message lu par etmi_dir.
*
* ARGUMENTS EN SORTIE :
* NEANT
* 
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
* - les champs de pa_DesSocks doivent contenir les descs. des sockets ouverts par etmi_dir.
* - pa_MsgSock doit contenir un message
*
* FONCTION 
*
* - identification de la socket active ( serveur ou client ).
* - traitement du message en fonction de la socket active.
* - reinit, sortie ou envoi message vers une des sockets en fonction du resultat
*   du traitement.
*
------------------------------------------------------*/
void edir_traitement_msg_sock( XZEXT_Msg_Socket *pa_MsgSock )
{
    static char *version = "$Id: etmi_dir.c,v 1.11 1996/05/23 17:12:27 volcic Exp $ : edir_traitement_msg_sock" ;
    
    XZEXT_Cmde_LCR 	vl_CmdeLCR;	/* structure depouillement message recu sur socket */
    XDY_Texte		vl_Texte;
    int vl_ValRet;
    char 		*pl_TrameLisible;
    EPROT_Conditions    vl_cond ;
    


    /*A
    * traitement message en fonction du type de la socket active
    */
    /*A CAS type socket active */
    switch( edir_detecte_type_sock() )
    {
    case XZEXC_SOCK_ACT_DONN :	/*A cas XZEXC_SOCK_ACT_DONN */
    	/*B detection type message recu d'un client TETEL sur une socket donnees */

   	/*B SI demande d'arret ALORS appel ecfg_sortir() */
    	if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_ARRET ) != NULL )
    	{
    	    XZST_03EcritureTrace( XZSTC_INFO, "edir_traitement_msg_sock : demande arret TETMi %d",
    	    				 vg_Config.no_ligne );
    	    ecfg_sortir();
    	}
    	/*B SINON SI demande init. ALORS appel ecom_init_TEL() */
    	else if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_INIT ) != NULL )
    	
       	{
    	    XZST_03EcritureTrace( XZSTC_INFO, "edir_traitement_msg_sock : demande initialisation TETMi %d",
    	    				 vg_Config.no_ligne );

    	    if ( (vg_EtatST = ecom_init_TEL( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
    	    	XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : relance init tetm%d", vg_Config.no_ligne );
    	    else   
    	    	XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : relance init tetm%d", vg_Config.no_ligne );
    	}

    	/*B SINON SI demande envoi trame TEL sur ligne serie geree 
    	 *  ALORS appel epro_envoi_trame_TEL() */
    	else if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_TEL ) != NULL )
    	{
    	    /*B SI depouillement message recu sur socket donnees : ecom_extraire_cmd_TEL() OK */
	    if( ecom_extraire_cmd_TEL( pa_MsgSock, &vl_CmdeLCR, vg_DesSocks.curr ) == XDC_OK )
	    {
	    	XZST_03EcritureTrace( XZSTC_INFO, "edir_traitement_msg_sock: Cmd TETEL <%s>", pa_MsgSock ); 
	    	/*B ALORS SI demande de mise ou d'annulation de mise en repondeur */
	    	/*B       ALORS epro_envoi_trame_REP() */
	    	vg_sock_TETEL = vg_DesSocks.curr;
	    	if ( vl_CmdeLCR.no_cmd != XDC_APPEL )
	    	{
	    	     epro_envoi_trame_REP( &vl_CmdeLCR );
	        }
	    	/*B SINON epro_envoi_trame_TEL() */
	    	else
	    	{
	    	     epro_envoi_trame_TEL( &vl_CmdeLCR );
	        }
	    }
	    /*B SINON return */
	    else
	    {
	    	return;
	    }
    	}
    	/*B SINON SI demande inconnue ( diff. de ARRET, INIT ou LCR ) 
    	 *  ALORS ecriture trace : " Recu demande TETEL inconnue : <demande_TETEL>" */
    	else
    	{
    	    XZST_03EcritureTrace( XZSTC_WARNING, " Recu demande TETEL inconnue : %s", pa_MsgSock->msg );
    	}
    	break;
    	
    case XZEXC_SOCK_ACT_LSER :	/*A cas XZEXC_SOCK_ACT_LSER */
    	/*B Normallement reception reponses equps. ds. epro_envoi_trame_TEL().
    	 *  si socket ligne serie active <=> reception reponse arrivee en retard.
    	 *  => ecriture trace : " Reponse equipement MAA  "
    	 */
    	/* if( (pl_TrameLisible = malloc( 24*pa_MsgSock->len + 1 )) )
     	{
     	    XZST_03EcritureTrace( XZSTC_WARNING, " Trame pour TEL perdue <%s> ", 
     			ecom_rendre_trame_lisible( pa_MsgSock->msg, pa_MsgSock->len, pl_TrameLisible ) );
     	    free( pl_TrameLisible );
     	}
     	*/
     	vl_cond.nb_essais = 1;
     	vl_cond.nb_err_crc = 0;
     	vl_cond.nb_non_rep = 0;
     	vl_CmdeLCR.sock_clnt = -1;
     	sprintf ( vl_CmdeLCR.rgs, "%d\0", vg_Config.no_ligne );
     	vl_CmdeLCR.no_cmd = 0;
     	epro_traitement_msg_lser( &vl_CmdeLCR, pa_MsgSock->msg, &vl_cond) ;
    	break;
    	
    default :	/*A defaut ( XZEXC_SOCK_ACT_UNDEF ) */
    	break;
    }
    	
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Detection type de la socket active
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int edir_detecte_type_sock( void )
*
* ARGUMENTS EN ENTREE :
*
* NEANT : utilise la var. globale vg_DesSocks.
* 
*
* ARGUMENTS EN SORTIE :
* NEANT
* 
* CODE RETOUR : 
*
*  XZEXC_SOCK_ACT_UNDEF	: si connexion client = evnmt. sur socket ecoute vg_DesSocks.ect
*  XZEXC_SOCK_ACT_LSER	: si evnmt. sur socket ligne serie geree 
*  XZEXC_SOCK_ACT_DONN	: si evnmt. sur un socket de comms. avec un client
*
* CONDITION D'UTILISATION
* 
*
* FONCTION 
*
*
------------------------------------------------------*/
int edir_detecte_type_sock( void )
{
    static char *version = "$Id: etmi_dir.c,v 1.11 1996/05/23 17:12:27 volcic Exp $ : edir_detecte_type_sock" ;
    int		vl_TypeSockAct	= XZEXC_SOCK_ACT_UNDEF;
    
    /*A
    * detection type socket active 
    */
    /*A SI socket RTServer active ALORS RETURN XZEXC_SOCK_ACT_RTSRV */
    if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
    {
    	return( XZEXC_SOCK_ACT_RTSRV );
    }
    /*A SI socket ligne serie active ALORS return XZEXC_SOCK_ACT_LSER */
    if( vg_DesSocks.curr == vg_DesSocks.lser )
    {
    	return( XZEXC_SOCK_ACT_LSER );
    }
    /*A SI socket ecoute => connexion client => ALORS return XZEXC_SOCK_ACT_UNDEF */
    if( vg_DesSocks.curr == vg_DesSocks.ect )
    {
     	return( XZEXC_SOCK_ACT_UNDEF );
    }
    
    /*A SINON => socket donnees (client TEzzz) active : return XZEXC_SOCK_ACT_DONN */
    return( XZEXC_SOCK_ACT_DONN );
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement evennement deconnexion client TEzzz.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void edir_traitement_dcnx( void )
*
* ARGUMENTS EN ENTREE :
*
* NEANT : utilise la var. globale vg_DesSocks et vg_SockList
* 
*
* ARGUMENTS EN SORTIE :
* NEANT
* 
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
* 
* Appel de cette fonction si la lecture sur une socket ds. la liste retourne XZEXC_DECONNEC.
*
* FONCTION 
*
* Envoi message de deconnexion 
* sans relance initialisation communication
*
------------------------------------------------------*/
void edir_traitement_dcnx( void )
{
    static char *version = "$Id: etmi_dir.c,v 1.11 1996/05/23 17:12:27 volcic Exp $ : edir_traitement_dcnx" ;

    XDY_Texte		vl_Msg;

    /*A ecriture trace : Deconnexion socket adresse <vg_DesSocks.curr_addr>...*/
    XZST_03EcritureTrace( XZSTC_INFO, " Deconnexion socket adr. <%s.>..", vg_DesSocks.curr_addr );
						
    /*A SI deconnexion socket serveur terminaux */
    if( vg_DesSocks.curr == vg_DesSocks.lser )
       vg_EtatST = XDC_NOK;
		    			
    /*A SI deconnexion socket RTWorks */
    if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
    {
    	/*B ALORS ecriture trace "Deconexion socket comms. avec RTServer"*/	    
	XZST_03EcritureTrace( XZSTC_INFO, 
		"Deconexion socket comms. avec RTServer !" );
    }

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement du message receptionne par etmi_dir sur la socket active.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void edir_traitement_liste_cmdes_TEL( void );
*
* ARGUMENTS EN ENTREE :
*
* NEANT : utilise la var. globale vg_DesSocks et vg_SockList
* 
*
* ARGUMENTS EN SORTIE :
* NEANT
* 
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
* 
* Appel de cette fonction si la lecture sur une socket ds. la liste retourne XZEXC_DECONNEC.
*
* FONCTION 
*
* Verifie quelle est la socket deconnectee.
* Si socket serveur terminaux deconnectee => envoi message a toutes les taches TEzzz.
* Si toutes les TEzzz se sont deconetcees, appel ecfg_sortir().
*
------------------------------------------------------*/

void edir_traitement_liste_cmdes_TEL( void )

{
    static char *version = "$Id: etmi_dir.c,v 1.11 1996/05/23 17:12:27 volcic Exp $ :  edir_traitement_liste_cmdes_TEL ";
    
    XZEXT_Cmde_LCR 	vl_CmdeLCR;	/* structure LCR pou cmde. TEL depilee */
    
    
    /*A depilement de la liste des cmdes. TEL a traiter utilisant la structure LCR */
    edir_traitement_liaison_ST( );
    
    /*A TANT QUE la liste des cmdes. n'est pa vide */
    while( pg_ListeCmdesLCR != NULL )
    {
      	/*B sortie de la 1ere cmde. ds. la liste : appel ecom_getfirst...() */
    	ecom_getfirst_cmd_LCR( &vl_CmdeLCR, &pg_ListeCmdesLCR );
    	
    	/*B envoi de la cmde. depilee sur la ligne serie : appel envoi epro_envoi_trame_TEL( ) */
	XZST_03EcritureTrace( XZSTC_INFO, "edir_traitement_liste_cmdes_TEL: Cmd TETEL <%s>", vl_CmdeLCR.cmd_LCR ); 
	/*B ALORS SI demande de mise ou d'annulation de mise en repondeur */
	/*B       ALORS epro_envoi_trame_REP() */
	if ( vl_CmdeLCR.no_cmd != XDC_APPEL )
	{
	   epro_envoi_trame_REP( &vl_CmdeLCR );
	}
	/*B SINON epro_envoi_trame_TEL() */
	else
	{
	    epro_envoi_trame_TEL( &vl_CmdeLCR );
	}
    }
    /*A FIN TANT QUE */	
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement de la connexion avec le serveur de terminaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void edir_traitement_liaison_ST( void );
*
* ARGUMENTS EN ENTREE :
*
* NEANT 
* 
*
* ARGUMENTS EN SORTIE :
* NEANT
* 
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
* 
* Appel de cette fonction si la liaison avec le serveur de terminaux est HS
*
* FONCTION 
*
* Essai de connexion sur la ligne serie
*
------------------------------------------------------*/

void  edir_traitement_liaison_ST( void )
{
    static char *version = "$Id: etmi_dir.c,v 1.11 1996/05/23 17:12:27 volcic Exp $ : edir_traitement_liaison_ST  ";

    XDY_Texte		vl_Texte = "";
    static int 		vl_CptTimeOut;		
    static int          vl_EtatLS = XDC_OK;

    if ( vl_CptTimeOut++ > (XZEXC_DUREE_ATTENTE*2) )
    {
       vl_CptTimeOut = 0;
       
       if ( vg_EtatST == XDC_OK )
       {
          if ( ex_cnx_test_ST (vg_Config.ligne_serie.NumeroST, vg_Config.no_port) == XDC_OK  )
          {
             vg_EtatST = XDC_NOK;
             if ( vg_DesSocks.lser > 0 )
             {
                ex_fin_cnx( vg_DesSocks.lser, &vg_SockList );
                vg_DesSocks.lser = XZEXC_SOCKET_NULL;
             }
          }
          XZST_03EcritureTrace( XZSTC_DEBUG2, "Etat de la liaison serie :%s", (vg_EtatST == XDC_OK) ?  "OK" : "NOK" );
       }

       if ( vg_EtatST != XDC_OK )
       {
          /*A Essai connexion avec ST */
          vl_CptTimeOut = 0;
          vg_EtatST = ecom_init_TEL( &vg_Config, &vg_SockList, &vg_DesSocks );
          sprintf( vl_Texte, "Liaison serie %d du Serveur de Terminaux %s %s !\0",
                         vg_Config.no_ligne,
                         vg_Config.ligne_serie.NumeroST,
                         (vg_EtatST == XDC_OK) ?  "retablie" : "coupee" );
      }

      if ( vl_EtatLS != vg_EtatST )
      {
         /*A message pour l'admin. systeme XZST_10ArchiverMsgSyst() : Etat connexion ST */
         XZST_10ArchiverMsgSyst( 0, vl_Texte, NULL );
         XZST_03EcritureTrace( XZSTC_WARNING, "%s", vl_Texte );
         vl_EtatLS = vg_EtatST;
      }
   }
    	
}


