/*E*/
/* Fichier : etel_dir.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TETEL * FICHIER etel_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module directeur de la tache TETEL
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	30 Nov 1994	: Creation
* Mismer.D      Version 1.2     28 Jan 1995     : RAS
* Fontaine.C    Version 1.3     03 Mai 1995     : Reprise entete et commentaire
* Fontaine.C    Version 1.4     17 Mai 1995     : Reprise historique
* Mismer.D  	version 1.5	07 Jun 1995	: Modif connexion socket RTserver
* Mismer.D  	version 1.6	31 Aou 1995	: Modif connexion socket RTserver suite
* Mismer.D  	version 1.7	11 Oct 1995	: Ajout appel fonction init Acknoledge alarme
* Diaz.L	version 1.8	15 Avr 1998	: Ajout des foctions pour piloter le PABX 
* CLuchague     version 1.11	23 Sep 2002	: Modif pour alcatel mandelieu                    
*
* C.Hilmarcher  version 1.12    28/10/2003      : Suite modif alcatel mandelieu
* SFO           06/10/14 : Adaptation pour compilation sous linux
 * JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.14
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "etel_dir.h"
/*#include "cstadrv.h"*/

/* definitions de constantes */

/* definitions de types locaux */

typedef struct {
	int		NumMsg;		/* Numero de message */
	XDY_Fonction	FoncCB;		/* Fonction a appeler */
}tm_callback;

/* definition de macro locales */

/* declaration de variables locales */
/*int		pabxIsAlive = FALSE;
unsigned short	callRefId;
unsigned short	deviceId;*/

static char *version = "@(#)etel_dir.c	1.12 12/04/03      : etel_dir" ;

static tm_callback pm_TabCB[] =
{
   { XDM_ECMD_EQPT, 		(void (*)())ed_controle_cb },
   { XDM_EINIT_ABT, 		(void (*)())ed_init_etat_AUTOTEL },
   { XDM_ECMD_AUTOTEL, 		(void (*)())ed_cmd_AUTOTEL },
};

/* declaration de fonctions internes */
/*PbxLink vg_Sock_Alcatel1;*/

/* definition de fonctions externes */


/*
--------------------------------------------
   SERVICE RENDU :
   Recuperation des Numero de poste ALCATEL
--------------------------------------------
*/
lire_num_po()
{
	int		vl_fd;
	char 		pl_PathEtFic[XDC_PATH_ABS_SIZE];
	XDY_Texte	pl_LigneFich;
	int		vl_NbParam  = 0;
	int		vl_int1 =0;
	int		vl_int2=0 ;
	
	/*A Ouverture du fichier de configuration des MAA */
	/* Construction du nom du fichier  */
	sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, "Config_ALCATEL_Numero" );
	
	/* Ouverture du fichier Config_ALCATEL_Numero  */
	if ( (vl_fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
	{
		/* Ecriture trace absence fichier */
		XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
		/* Sortie de la fonction */
         	return ( XDC_NOK );
	}
	
	/*A Lecture de chaque ligne du fichier */
	while ( ex_LireLigneFich ( vl_fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
	{
		/* Lire ligne suivante si c'est une ligne de commentaire */
		if ( *pl_LigneFich != XDF_COMMENTAIRE )
		{
			/*Extraire les numeros*/ 
			vl_NbParam = sscanf ( pl_LigneFich, "%d %d",
							&vl_int1,
							&vl_int2
							);
			/*B Si tout les parametre ont ete lu */
			if ( vl_NbParam == 2)
			{
				 if (pg_debutMAA->Config.NumeroPO == vl_int1)
				 {
					sprintf(vg_Numero_PO, "%s", vl_int2) ;
				 }
			}
			
		}
	}
	close(vl_fd);
}


/*---------------------------------
   SERVICE RENDU :
   Recuperation du message ALCATEL
-------------------------------------
*/
#ifdef _HPUX_SOURCE
CstaMsg *waitCstaMessage(CstaCrid  crid)
{
	CstaMsg   *message;
        PbxLink   linkrep;
        
        while (1)
        {
        	message = LookForFrame(NULL, 6000000, (Voidfun) FrameRead, &linkrep);
        	if (message != NULL)
        	{
        		if ((crid == 0) || (RoseId(message->serv) == crid))
        		{
        			return message;
        		}
        	}
        }
}
#endif

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
* Programme principal
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void main( argc, argv )

/*
* ARGUMENTS EN ENTREE :
*   argc : Nombre d'arguments
*   argv : liste des arguments
*
* ARGUMENTS EN SORTIE :
*   Aucune
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
* Initialisations :connexion rtserver, connection avec les taches TESRV
* init des structures en memoire, gestion des callbacks
* 
* Boucle sur les sockets et gestion des messages recus 
* 
------------------------------------------------------*/

int	argc;
char	**argv;
{
	int 			vl_ValRet	=XDC_OK,          	      	
				vl_SockRet	= 0,
				vl_SockSRV	= 0,
				vl_resultat;
	XZEXT_MSG_SOCKET	pl_MsgTimer;
   	XDY_Datagroup		pl_NomDG;
	int			vl_indice_callback;
	XZEXT_MASK		vl_masque;
	int			vl_Type_Machine;
#ifdef _HPUX_SOURCE
	CstaMsg                 *msge; 
#endif
	ETEL_DONNEES_MAA *	pl_listeMAA;
	XDY_Tel			vl_EtatRepondeur = "";
	XDY_Datagroup		pl_DG;
	XDY_Eqt			vl_po = 0;
	XDY_NomMachine		vl_NomAutocom = "";
	int			vl_pass = 9;
	int			vl_pass_av = 10;
	
	/*A
	 *  Recuperation du nom de la machine 
	 */
	strcpy(vg_NomMachine,argv[1]);
	
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A
	 * Connexion au RTserver et initialisation des variables
	 */
	if( XZSC_01InitCnxSup(  NULL,
				argv[ 0 ],
				XZSCC_INTERDIRE,
				XZSCC_AUTORISER,
				XZSCC_INTERDIRE,
				ec_sortir,
				"/dev/null" ) != XDC_OK )
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion avec le RTserver." );
	   exit( XDC_NOK ); 
	}
	else
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
	}

	/*A
	 * Recuperation du site de la machine 
	 */
	if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible . " );
	} 

	/*A Creation du repertoire pour les sockets */
	ex_rep_socket ( argv[1] );
	
	/*A Init pour Acknoledge Alarme */
	ex_init_alarme ( argv[1], argv[0] );

	/*A 
	 * Connexion socket en mode client pour communiquer avec la tache TESRV 
	 */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TETEL,
				XZEXC_ADR_SOCK_TESRV,
				&vg_SockMask,
				&vl_SockSRV ) != XDC_OK )
	{ 
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client  ." );	
	}
        
	/*A 
	 * Appel de MCFG fonction ec_init 
	 */
        ec_init(vg_NomMachine);

	 /* Arecuperation du type de machine */
	if ( XZSA_17TypeMachine( vg_NomMachine, &vl_Type_Machine ) != XDC_OK )
	{
	    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de trouver le type de la machine" );
	}
	/* verrue cluchague */
        vl_Type_Machine = XZSCC_TYPEM_PC2;	
	/* fin verrue cluchague */
	if ( ( vl_Type_Machine == XZSCC_TYPEM_PC2 ) || ( vl_Type_Machine == XZSCC_TYPEM_PC2LT )  )
	{
	    vg_Mac_PC2 = XDC_VRAI;
	    lire_num_po();
#ifdef _HPUX_SOURCE
	    /* connexion a ALCATEL */
	    CsthInit();
	    sprintf ( vl_NomAutocom, "TEL%s", vg_NomSite );
	    if( SendConnect (vl_NomAutocom, CSTA_TCP_PORT, &vg_Alcatel) != STA_OK)
	    {
	    XZST_03EcritureTrace( XZSTC_WARNING, "PAS recuperation socket ALCATEL" );
	    }
	    else
	    {
	    XZST_03EcritureTrace( XZSTC_WARNING, "OK recuperation socket ALCATEL" );
	    }
	    vg_Sock_Alcatel = pbxSock[vg_Alcatel];
	    XZST_03EcritureTrace( XZSTC_WARNING, "Socket ALCATEL %d", vg_Sock_Alcatel);
	    if (vg_Sock_Alcatel >  0)
	    {
	       /* Insertion de la socket (lien) ALCATEL dans le masque des socket */
	       XZST_03EcritureTrace( XZSTC_WARNING, "OK recuperation socket ALCATEL %d", vg_Sock_Alcatel);
	       SendMonitorStart(vg_Alcatel, vg_Numero_PO, ROSE_NONE, 0, 0, NULL);
	       SendMakeCall(vg_Alcatel, vg_Numero_PO, "790", ROSE_NONE, NULL);
	       vl_masque.desc_sock = vg_Sock_Alcatel;
	       strcpy( vl_masque.adresse, "Alcatel" );
	       ex_ins_masque ( &vl_masque, &vg_SockMask );
	    }
	    else
	    {
	       XZST_03EcritureTrace( XZSTC_WARNING, "Echec recuperation socket ALCATEL %d", vg_Sock_Alcatel);
	       sleep(5);
	    }
#endif
	}

	/*A 
	 * Armement des Callback sur reception des message 
	 */
	 
         /* Initialisation des callbacks de la tache */
	
         for (vl_indice_callback = 0; vl_indice_callback < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_indice_callback++)
	{
	   if(!TipcSrvProcessCbCreate(	XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg ) ,
						pm_TabCB[vl_indice_callback].FoncCB ,
						(T_CB_ARG)NULL) )
	   {
      	      XZST_03EcritureTrace(XZSTC_WARNING,
					"erreur init callback '%s'",
					pm_TabCB[vl_indice_callback].FoncCB);
	   }
	}
	
	/*A 
	 * Abonnement aux Datagroup 
	 */
	/* Messages de controle : XDG_ECMD_AUTOTEL_<NomSite> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_AUTOTEL, vg_NomSite) ;			
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_WARNING,
	   "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	}			

	/* Messages de demande de lecture infos MAA : XDG_EINIT_ABT_AUTOTEL_<NomSite> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT_ABT_AUTOTEL, vg_NomSite) ;			
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_WARNING,
	   "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	}
				
	
	/*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
	while (1)
	{
	   if ( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK )
	   {
              XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select  ." );
#ifdef _HPUX_SOURCE
              if ( ( vl_SockRet == vg_Sock_Alcatel ) && ( vg_Sock_Alcatel != 0 ) )
              {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fontion select sur socket ALCATEL" );
                  ex_fin_cnx ( vg_Sock_Alcatel, &vg_SockMask );
                  vg_Sock_Alcatel = 0;
              }
              else
              {
                  ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
              }
#else
                  ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
#endif
           }
           else
           {
            /*  XZST_03EcritureTrace(XZSTC_WARNING, " vl_SockRet = %d;  vg_Sock_Alcatel = %d", vl_SockRet, vg_Sock_Alcatel); */
#ifdef _HPUX_SOURCE
              if ( vg_Sock_Alcatel <= 0 )
              {
              /*conection alcatel*/
	         if ( ( vl_Type_Machine == XZSCC_TYPEM_PC2 ) || ( vl_Type_Machine == XZSCC_TYPEM_PC2LT )  )
	         {
	         	CsthInit();
	        	 if( SendConnect (vl_NomAutocom, CSTA_TCP_PORT, &vg_Alcatel) != STA_OK)
	        	 {
	        	    XZST_03EcritureTrace( XZSTC_WARNING, "PAS recuperation socket ALCATEL" );
	        	 }
	        	 else
	       		 {
	       		    XZST_03EcritureTrace( XZSTC_WARNING, "OK recuperation socket ALCATEL");
	         	 }
	         	vg_Sock_Alcatel = pbxSock[vg_Alcatel];
	         	XZST_03EcritureTrace( XZSTC_WARNING, " Socket ALCATEL %d", vg_Sock_Alcatel);
	         }
                 if( vg_Sock_Alcatel > 0 )
                 {
                    SendMonitorStart(vg_Alcatel, vg_Numero_PO, ROSE_NONE, 0, 0, NULL);
                    SendMakeCall(vg_Alcatel, vg_Numero_PO, "790", ROSE_NONE, NULL);
                    vl_masque.desc_sock = vg_Sock_Alcatel;
                    strcpy ( vl_masque.adresse, "Alcatel" );
                    ex_ins_masque ( &vl_masque, &vg_SockMask );
                 }
                 else
                 {
                     XZST_03EcritureTrace(XZSTC_WARNING, "Echec recuperation socket ALCATEL ." );
                    sleep(5);
                 }
              }
#endif
                ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );
	      /* Si une socket a ete activee */
	      if ( vl_SockRet != XZEXC_NO_EVT )
	      {
	         /* Selon le type de socket */
	         /* Socket RT server */
	         if ( vl_SockRet == vg_SockRT ) 	
	         {
	            /* Le message est lu par les primitive standard de RTWorks */
	         }
	         else
	         {
                 /* Socket TESRV (Init ou Arret tache) */
	            if ( vl_SockRet == vl_SockSRV )
	            {	
	            /* Appel de la fonction de controle et
	               de configuration de MCFG */
	                XZST_03EcritureTrace(XZSTC_INFO,
					"Info *** Init ou arret tache sur socket %d ",
					vl_SockRet ) ;
					
	              ec_controle ( vl_SockSRV, vg_NomMachine );
	            }
	            else
	            {
	               /* Selon type de socket */
#ifdef _HPUX_SOURCE
	               if ( vl_SockRet == vg_Sock_Alcatel )
	               {
	                  /* Socket ALCATEL */
	                  msge = waitCstaMessage (0);
	                  if ( msge != NULL )
	                  {
	                    	pl_listeMAA = pg_debutMAA; 
				switch (msge->type_msg) 
	  			{
	  				case CSTAEVTCMAGIC :
 	  					XZST_03EcritureTrace( XZSTC_WARNING, "ALCATEL - type_msg = CSTAEVTCMAGIC " );
 	  					vl_pass = 0;
 	  					pl_listeMAA->CptRenduCde = XDC_OK;
	  					if ( CstaEvent(msge->evtc) == EVT_CONNECTION_CLEARED )
		  				{
		  					XZST_03EcritureTrace( XZSTC_WARNING, "ALCATEL - msg->evtc = EVT_CONNECTION_CLEARED " );
		  					vl_pass = 1;
		  					pl_listeMAA->CptRenduCde = XDC_ATTENTE;
		  				}
		  				else
		  				{
	  						if ( CstaEvent(msge->evtc) == EVT_SERVICE_INITIATED )
	  						{  
	  							XZST_03EcritureTrace( XZSTC_WARNING, "ALCATEL - msg->evtc = EVT_SERVICE_INITIATED " );
	  							vl_pass = 2;
	  							pl_listeMAA->CptRenduCde = XDC_OK;
	  						}
	  						else
	  						{
	  							if ( CstaEvent(msge->evtc) == EVT_DELIVERED )
		  						{
		  							XZST_03EcritureTrace( XZSTC_WARNING, "ALCATEL - msg->evtc = EVT_DELIVERED " );
		  							vl_pass = 3;
		  							pl_listeMAA->CptRenduCde = XDC_RING;
		  						}
		  						else
		  						{
		  							if ( CstaEvent(msge->evtc) == EVT_ESTABLISHED )
		  							{
		  								XZST_03EcritureTrace( XZSTC_WARNING, "ALCATEL - msg->evtc = EVT_ESTBALISHED " );
		  								vl_pass = 4;
		  								pl_listeMAA->CptRenduCde = XDC_CONNECT;
		  							}
		  						}
	  						}
	  					}
	  					break;
	  				case CSTASREPMAGIC :
	  					XZST_03EcritureTrace( XZSTC_WARNING, "ALCATEL - type_msg = CSTASREPMAGIC " );
	  					vl_pass = 5;
	  					pl_listeMAA->CptRenduCde = XDC_NOANSWER;
	  					break;
	  				case CSTAERREMAGIC :
	  					XZST_03EcritureTrace( XZSTC_WARNING, "ALCATEL - type_msg = CSTAERREMAGIC " );
	  					vl_pass = 6;
	  					pl_listeMAA->CptRenduCde = XDC_ERROR;
	  					break;	
	  				case ACSERLRQMAGIC :
	  					XZST_03EcritureTrace( XZSTC_WARNING, "ALCATEL - type_msg = ACSERLRQMAGIC " );
	  					vl_pass = 7;
	  					break;
	  				case ACSEABRTMAGIC :
	  					XZST_03EcritureTrace( XZSTC_WARNING, "ALCATEL - type_msg = ACSEABRTMAGIC " );
	  					vl_pass = 8;
	  					break;	
	  				default :
						vl_pass = 9;
	  					break;
	  			}
	  			
			        if ( pl_listeMAA->RenvoiSurRepondeur ==  XDC_VRAI )
	    			{ 
               				strcpy ( vl_EtatRepondeur, XDC_MARCHE );
            			}
            			else
	    			{ 
               				strcpy ( vl_EtatRepondeur, XDC_ARRET );
           			}
            			if ( pl_listeMAA->NoCdeCourante == XDC_APPEL )
	    			{ 
               				vl_po = pl_listeMAA->Config.NumeroPO;
            			}
            			else
	    			{ 
               				vl_po = 0;
            			}
            			if ( vl_pass != vl_pass_av )
            			{
            				XDG_EncodeDG2 ( pl_DG, XDG_EETAT_AUTOTEL, vg_NomSite) ;
            				XZST_03EcritureTrace( XZSTC_WARNING, "demande animation SAE : %d -- %d ", vl_pass, vl_pass_av);
            				vl_pass_av = vl_pass;
            				if (!TipcSrvMsgWrite ( pl_DG,			
   	 		         	  XDM_IdentMsg(XDM_ETAT_AUTOTEL),
        		      	 	  XDM_FLG_SRVMSGWRITE,
       				 	  T_IPC_FT_INT4,       vl_po,
       				  	 T_IPC_FT_INT4,       pl_listeMAA->NoCdeCourante,
       				  	 T_IPC_FT_STR,        pl_listeMAA->NoTelAppele,
          			  	 T_IPC_FT_STR,        vl_EtatRepondeur,
             	  		  	 T_IPC_FT_INT4,       pl_listeMAA->CptRenduCde,
                   		  	 T_IPC_FT_INT4,  	pl_listeMAA->EtatLiaison,
                  		  	 NULL))
      			        	{
 	                          	 XZST_03EcritureTrace(XZSTC_WARNING,"Envoi message XDM_ETAT_AUTOTEL non effectue." );
         		        	}
                                	else
                                	{
                                 	  XZST_03EcritureTrace(XZSTC_INFO
 		                        	               ,"Envoi message XDM_ETAT_AUTOTEL effectue sur   %d - %d - %d " ,
 		                       		                pl_listeMAA->NoCdeCourante,
 		                        	               	pl_listeMAA->CptRenduCde,
                                        	    		pl_listeMAA->EtatLiaison);
                               		}
                               }  
	                  }
	               }
	               else
	               {
	                  /* Appel analyse des retours traitement de la trame */
	                  et_analyser_trame( vl_SockRet ); 
	               }
#else
		       /* Appel analyse des retours traitement de la trame */
		       et_analyser_trame( vl_SockRet ); 
#endif
	            }
		 }
	      }   /* fin si socket active */
	   }   

	   if (XZSS_15ConsulterTimeout () == XDC_OUI)
	   {
	      XZSS_18AnnulerTimeout ();
	      ex_traiter_alarmes ();
	   }
	}   /* fin while */

}  /* Fin main */


