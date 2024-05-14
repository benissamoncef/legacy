/*E*/
/* Fichier : $Id: xzec.c,v 1.23 2019/01/29 11:25:55 pc2dpdy Exp $     Release : $Revision: 1.23 $      Date :  $Date: 2019/01/29 11:25:55 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER xzec.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface Commun >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	24 Aug 1994	: Creation
* Mismer.D	version 1.2	28 Sep 1994	:
* Mismer.D	version 1.3	28 Sep 1994	:
* Nagiel.E	version 1.4	10 Oct 1994	: Ajout de XZEC02
* Mismer.D	version 1.5	21 Oct 1994	: Ajout des Bouchons XZEC20 XZEC21 XZEC22 XZEC23 XZEC24 
* Mismer.D	version 1.6	02 Dec 1994	:
* Volcic.F	version 1.7	28 Dec 1994	:
* Mismer.D	version 1.8	29 Dec 1994	: Ajout des XZEC20 XZEC21 XZEC22
* Mismer.D	version 1.9	06 Jan 1995	: Ajout des type d'equipement manquant XZEC03 XZEC04
* Mismer.D	version 1.10	27 Fev 1995	: Ajout XZEC06_Inhibition_Eqt
* Fontaine.C	version 1.11	24 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.12	07 Nov 1996	: Modif METO (DEM/1232)
* Mismer.D	version 1.13	10 Fev 1997	: Ajout constantes TDP (DEM/1395)
* Mismer.D	version 1.14	20 Fev 1997	: Modif constantes TDP (DEM/1396)
* Mismer.D	version 1.15	15 Jan 1998	: Modif constantes PAL (DEM/)
* JMG		version 1.16	13/09/05	: ajout PMVA BAD BAF
* JPL		06/12/10 : Migration architecture HP ia64 (DEM 961) : taille et adresse tampons de lecture chaines  1.17
* JPL		06/12/10 : Migration architecture HP ia64 (DEM 961) : '=' au lieu de '==' (!!! effet de bord !!!)  1.18
* VR		09/12/11 : Ajout PRV (DEM/1016)
* PNI		07/02/13 : Correction trace en debut de XZEC02 FI 1813  v1.20
* JPL		06/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.21
* JPL		07/02/18 : Migration Linux 64 bits (DEM 1274) : Concordance constantes/types; test de validite fichiers  1.22
------------------------------------------------------*/

/* fichiers inclus */


#include "xzec.h"

/* definitions de constantes */
#define	XZECC_TAILLE_TAMPON	24		/* pour lecture des chaines de caracteres */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzec.c	1.14 04/07/97 : xzec" ;

/* declaration de fonctions internes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction appel≈e pour lire une ligne dans un fichier ASCII
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEC_LireLigneFich ( 	int 	va_Fd,
				char	*pa_Ligne,
				int	va_NbCar )


/*
* ARGUMENTS EN ENTREE :
*  va_Fd		File descripteur du fichier
*  va_NbCar		Taille maxi de la ligne
*
* ARGUMENTS EN SORTIE :
*  pa_Ligne		Contenu de la ligne lu
*
* CODE RETOUR : 
*  int			Nombre caracteres retourne
*  XZEXC_EOF		Fin du fichier atteinte
*  XDC_NOK		echec lecture ligne
*
* CONDITION D'UTILISATION
*
* FONCTION 

*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC_LireLigneFich " ;

int			vl_ValRet;
XDY_Texte		pl_Ligne = {0};
int 			vl_Size = sizeof ( pl_Ligne );
int			vl_NbCarLu = 0;
int			vl_LongLigne;

   if ( va_NbCar < sizeof ( pl_Ligne ) )
   {
      /*A Mise a jour du nombre de caractere a lire */
      vl_Size = va_NbCar;
   }
        
   /*A Lecture du fichier */
   if ( ( vl_NbCarLu = read ( va_Fd, pl_Ligne, vl_Size ) ) > 0)
   {
      /*A Recherche de la longueur de la ligne */
      vl_LongLigne = strcspn( pl_Ligne, XZEXC_LINE_FEED);
      /*A Recopie du contenu des caractere lu pour l'appelant */
      pl_Ligne[vl_LongLigne + 1] = '\0';
      memcpy ( pa_Ligne, pl_Ligne, strlen(pl_Ligne));
      strcpy ( pa_Ligne, pl_Ligne);  
      /*A Positionnement du pointeur du fichier au debut de la ligne suivante */
      lseek ( va_Fd, vl_LongLigne + 1 - vl_NbCarLu , SEEK_CUR );
      /*A Recopie du nombre de caractere lu pour le code retour de la fonction */
      vl_ValRet = vl_LongLigne;
   }
   else
   {
      /*A SI le nombre caractere lu est nul */
      if ( vl_NbCarLu == 0)
      {
         /*A Retourner fin de fichier atteinte */
         vl_ValRet = XZEXC_EOF;
      }
      else
      {
         /*A Retourner erreur lecture fichier */
         vl_ValRet = XZEXC_EOF;
      }
   }
   return ( vl_ValRet );  
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction retournant le nom de l'equipement correspondant
*  au type d'equipement fournit en entree
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC_nom_eqt ( int	va_TypeEqt , char	*pa_NomEqt )

/*
* ARGUMENTS EN ENTREE :
*  va_TypeEqt		: Type d'equipement
*
*
* ARGUMENTS EN SORTIE :
*  paNomEqt		: Nom de l'equipement
*
*
* CODE RETOUR : 
*  XDC_OK		: Succes
*  XDC_NOK		: Echec
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC_nom_eqt ";
int	vl_ValRet = XDC_OK;

	/*A Selon le type d'equipement recopie du nom de l'quipement dans l'argument de sortie */
  	switch ( va_TypeEqt )
  	{
   	case 	XDC_EQT_RAU	: strcpy ( pa_NomEqt, XZECC_NOM_RAU );
   			  	  break;
   	case 	XDC_EQT_GEN_LCR	: strcpy ( pa_NomEqt, XZECC_NOM_QTX );
   			  	  break;
   	case 	XDC_EQT_TDP	: strcpy ( pa_NomEqt, XZECC_NOM_TDP);
   			  	  break;
   	case 	XDC_EQT_ZDP	: strcpy ( pa_NomEqt, XZECC_NOM_TDP);
   			  	  break;
   	case 	XDC_EQT_PAU	: strcpy ( pa_NomEqt, XZECC_NOM_RAU );
   			  	  break;
   	case	XDC_EQT_MAA	: strcpy ( pa_NomEqt, XZECC_NOM_TEL );
   				  break;
	case	XDC_EQT_OPE	: strcpy ( pa_NomEqt, XZECC_NOM_OPE );
				  break;
	case	XDC_EQT_SYV	: strcpy ( pa_NomEqt, XZECC_NOM_VID );
				  break;
	case	XDC_EQT_LTV	: strcpy ( pa_NomEqt, XZECC_NOM_VID );
				  break;
	case	XDC_EQT_CAM	: strcpy ( pa_NomEqt, XZECC_NOM_VID );
				  break;
	case	XDC_EQT_MAG	: strcpy ( pa_NomEqt, XZECC_NOM_VID );
				  break;
	case	XDC_EQT_MNT	: strcpy ( pa_NomEqt, XZECC_NOM_VID );
				  break;
	case	XDC_EQT_TUBE	: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
				  break;
	case	XDC_EQT_ECH	: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
				  break;
	case	XDC_EQT_CPT	: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
				  break;
	case	XDC_EQT_PAN	: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
				  break;
	case	XDC_EQT_LIT	: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
				  break;
	case	XDC_EQT_EXT	: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
				  break;
	case	XDC_EQT_CCO	: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
				  break;
	case	XDC_EQT_OPA	: strcpy ( pa_NomEqt, XZECC_NOM_NIC );
				  break;
	case	XZECC_TYPE_EQT_SAT	: strcpy ( pa_NomEqt, XZECC_NOM_SAT );
				  break;
	case	XDC_EQT_RAD	: strcpy ( pa_NomEqt, XZECC_NOM_RDT );
				  break;
	case	XDC_EQT_DAI	: strcpy ( pa_NomEqt, XZECC_NOM_DAI );
			  	  break;
	case	XDC_EQT_ANA	: strcpy ( pa_NomEqt, XZECC_NOM_DAI );
			  	  break;
	case	XDC_EQT_PMV	: strcpy ( pa_NomEqt, XZECC_NOM_PMV );
			  	  break;
	case	XDC_EQT_PCT	: strcpy ( pa_NomEqt, XZECC_NOM_PICTO );
			  	  break;
	case	XDC_EQT_NAV	: strcpy ( pa_NomEqt, XZECC_NOM_NAV );
			  	  break;
	case	XDC_EQT_FAC	: strcpy ( pa_NomEqt, XZECC_NOM_NAV );
			  	  break;
	case	XDC_EQT_FAU	: strcpy ( pa_NomEqt, XZECC_NOM_NAV );
			  	  break;
	case	XDC_EQT_PAL	: strcpy ( pa_NomEqt, XZECC_NOM_PAL );
			  	  break;
	case	XDC_EQT_GTC	: strcpy ( pa_NomEqt, XZECC_NOM_GTC );
			  	  break;
	case	XDC_EQT_TOR	: strcpy ( pa_NomEqt, XZECC_NOM_GTC );
			  	  break;
	case    XDC_EQT_PMVA     : strcpy ( pa_NomEqt, XZECC_NOM_PMVA );
                                  break;
	case    XDC_EQT_PRV     : strcpy ( pa_NomEqt, XZECC_NOM_PRV );
                                  break;
	case    XDC_EQT_BAF     : strcpy ( pa_NomEqt, XZECC_NOM_BAF );
                                  break;
	case    XDC_EQT_BAD     : strcpy ( pa_NomEqt, XZECC_NOM_BAD );
                                  break;
	case 	XDC_EQT_CAP	: strcpy ( pa_NomEqt, XZECC_NOM_CAP);
				break;
	case 	XDC_EQT_CFE 	: strcpy ( pa_NomEqt, XZECC_NOM_CFE);
				break;
   	default			: strcpy ( pa_NomEqt, XZECC_NOM_QTX );
   			  	  break;
	}
	
	return ( vl_ValRet );
      
}

/* definition de fonctions externes */


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

int XZEC00_Verif_Coherence_Nombre ( char *pa_String, XDY_Entier va_XZECC_Type, XDY_Entier va_Type ) 
				        
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
   static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC00_Verif_Coherence_Nombre" ;

   XDY_Entier  	vl_retcode = 0;
   XDY_Coeff	vl_valeur_nbre = 0;
   XDY_Entier	i = 0;
   XDY_Entier	j = 0;
   XDY_Booleen  vl_finligne;
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN : XZEC00_Verif_Coherence_Nombre : String = %s et LongueurType = %d", pa_String, va_XZECC_Type );
   
   /*A
   ** On XZEC00_verifie que la chaine ne possede pas de caractere "\n" 
   */
   
   vl_finligne = XDC_FAUX;
   
   for ( j = 0; j < strlen ( pa_String ) ; j ++ )
   {
       if ( pa_String[j] == XZECC_LINE_FEED )
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
      
      if ( strlen ( pa_String ) >  va_XZECC_Type )
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
      
      while ( pa_String[i] != XZECC_FIN_STRING )
      {
         
         if ( ( ( pa_String[i] < '0' ) || ( pa_String[i] > '9' ) ) && ( pa_String[i] != '.' ) ) 
         {
            /*cgcd_BilanConf_Trace( pa_String );*/
            printf ("\n***  %s  ***\n", pa_String );
            return ( XDC_NOK );
         } 
         
         i++;  
              
      }
      
      /*B
      ** Si OK : mise au format long de la valeur de pa_String
      ** XZEC00_Verification en fonction du type pour ne pas depasser la valeur maximale
      */
      
      sscanf ( pa_String, "%f", &vl_valeur_nbre );
      
      switch ( va_Type )
      {
      	case XZECC_DIV : 	if ( vl_valeur_nbre > XZECC_MAX_DIV )
      			       	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_DIV );
      			           return ( XDC_NOK );
   		               	}
   		               	break;
      
       case XZECC_COEFF : 	if ( vl_valeur_nbre > XZECC_MAX_COEFF )
      			       	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_COEFF );
      			           return ( XDC_NOK );
   		               	}
   		               	break;
      
       case XZECC_SEUIL_V :if ( vl_valeur_nbre > XZECC_MAX_SEUIL_V )
      			  	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_SEUIL_V );
      			           return ( XDC_NOK );
   		          	}
   		         	 break;
   		          
       case XZECC_SEUIL_Q :if ( vl_valeur_nbre > XZECC_MAX_SEUIL_Q )
      			  	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_SEUIL_Q );
      			           return ( XDC_NOK );
   		          	}
   		         	 break;
       case XZECC_SEUIL_TO:if ( vl_valeur_nbre > XZECC_MAX_SEUIL_TO )
      			  	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_SEUIL_TO );
      			           return ( XDC_NOK );
   		          	}
   		         	 break;
       	          
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
      
      if ( strlen ( pa_String ) > ( va_XZECC_Type + 1 ) )
      {
         printf ("\n***  %s  ***\n", pa_String );
         return ( XDC_NOK );
      } 
      
      else
      
      /*B
      ** Si les caracteres trouves ne sont pas compris entre 0 et 9
      ** Erreur de saisie : chaine trop longue
      ** return ( XDC_NOK ) and pa_String_out = null
      */
      
      while ( pa_String[i] != XZECC_LINE_FEED )
      {
         
         if ( ( ( pa_String[i] < '0' ) || ( pa_String[i] > '9' ) ) && ( pa_String[i] != '.' ) ) 
         {
            printf ("\n***  %s  ***\n", pa_String );
            return ( XDC_NOK );
         } 
         
         i++;  
              
      }
      
      /*B
      ** Si OK : mise au format long de la valeur de pa_String
      ** XZEC00_Verification en fonction du type pour ne pas depasser la valeur maximale
      */
      
      sscanf ( pa_String, "%f", &vl_valeur_nbre );
      
      switch ( va_Type )
      {
      	case XZECC_DIV : 	if ( vl_valeur_nbre > XZECC_MAX_DIV )
      			       	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_DIV );
      			           return ( XDC_NOK );
   		               	}
   		               	break;
      
       case XZECC_COEFF : 	if ( vl_valeur_nbre > XZECC_MAX_COEFF )
      			       	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_COEFF );
      			           return ( XDC_NOK );
   		               	}
   		               	break;
      
       case XZECC_SEUIL_V :if ( vl_valeur_nbre > XZECC_MAX_SEUIL_V )
      			  	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_SEUIL_V );
      			           return ( XDC_NOK );
   		          	}
   		         	 break;
   		          
       case XZECC_SEUIL_Q :if ( vl_valeur_nbre > XZECC_MAX_SEUIL_Q )
      			  	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_SEUIL_Q );
      			           return ( XDC_NOK );
   		          	}
   		         	 break;
       case XZECC_SEUIL_TO:if ( vl_valeur_nbre > XZECC_MAX_SEUIL_TO )
      			  	{
      			           XZST_03EcritureTrace(XZSTC_MESSAGE,"XZEC00_Verif_Coherence_Nombre : Nombre saisie = %ld est superieur au max du type = %ld", vl_valeur_nbre, XZECC_MAX_SEUIL_TO );
      			           return ( XDC_NOK );
   		          	}
   		         	 break;
      }
      
   }
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZEC00_Verif_Coherence_Nombre retourne %d ", XDC_OK);
   return ( XDC_OK );
     
}  
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de configuration d'un ou de tous les types 
*  d'equipements externes d'une ou de toutes les machines
*  d'un ou de tous les sites
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC01_Config (		XDY_NomSite	pa_NomSite,
				XDY_NomMachine	pa_NomMachine,
				int		va_TypeEquipement )		

/*
* ARGUMENTS EN ENTREE :
*   pa_NomSite		nom du site ( PC district ou LT )
*   pa_NomMachine	nom de la machine
*   va_TypeEquipement	type d'equipt a configurer (XZECC_TYPE_EQT_ )
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*
* CONDITION D'UTILISATION
*   Doit etre invoquee lors d'une modification dans la configuration 
*   d'au moins un equipement. 
*
* FONCTION 
*   Construction du message XDM_ECONFIG_EQPT en specifiant le type 
*   de configuration et en fournissant les valeurs de TypeEquipement,
*   NomMachine, NomSite recues.
*   Envoi du message sur le DataGroup XDG_ECONFIG_EQPT_<NomSite>
*
------------------------------------------------------*/
{

static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC01_Config " ;
XDY_Datagroup	pl_NomDG;


   /*A Trace des parametres en entree */
   XZST_03EcritureTrace (	XZSTC_FONCTION,
 				"IN: XZEC01_Config avec %s %s %d ",
 				pa_NomSite,
 				pa_NomMachine,
 				va_TypeEquipement );

   /* Construction du DatGroup XDG_ECONFIG_EQPT_<NomSite> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECONFIG_EQPT, pa_NomSite ) ;	
   /* Envoi du message XDM_ECONFIG_EQPT en fournissant les valeur recues */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECONFIG_EQPT)
                          ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_STR,strtok ( pa_NomMachine, " " )
                          ,T_IPC_FT_INT4,va_TypeEquipement
                          ,NULL))
      	{
         	XZST_03EcritureTrace(XZSTC_WARNING
 		,"Envoi message XDM_ECONFIG_EQPT non effectue ." );
 		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC01_Config avec XDC_NOK");
 		return ( XDC_NOK );
         				
	}
   TipcSrvFlush();
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC01_Config avec XDC_OK");
   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de changement de contraste (surbrillance).
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC02_Commande_Contraste (		int		va_Contraste,
					XDY_Eqt		va_NoEqt,
					XDY_TypeEqt	va_TypeEqt,
					XDY_NomSite	va_NomSite )		

/*
* ARGUMENTS EN ENTREE :
*   va_Contraste,	Surbrillance ou non
*   va_NoEqt		Surbrillance pour 1 eqt 
*                       soit pour tous les eqt du site avec XZEPC_SURB_EQTALL
*   va_NomSite		Nom du site
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_Contraste en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_CONTRASTE_<NomMachine> du site concerne
*
------------------------------------------------------*/
{
  static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC02_Commande_Contraste " ;

  XDY_Datagroup	     		pl_NomDG;
   
    /*A Trace des parametres en entree */
    
   XZST_03EcritureTrace(XZSTC_INTERFACE
 	,"IN: XZEP01_Commande_Contraste avec %d %d %d %s"
        ,va_Contraste
        ,va_NoEqt
        ,va_TypeEqt
        ,va_NomSite);
 	

   /*A Construction du DatGroup XDG_ECMD_CONTRASTE_<NomSite> */
   
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_CONTRASTE, va_NomSite ) ;
   	
   /*A Envoi du message XDM_ECMD_Contraste en fournissant les valeur recues  */
   
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_Contraste)
  			  ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT2, va_Contraste
                          ,T_IPC_FT_INT2, va_NoEqt
                          ,T_IPC_FT_INT2, va_TypeEqt
                          ,NULL))
      	{
  		/*A Trace envoi message XDM_ECMD_Contraste non effectue */    	
      	
         	XZST_03EcritureTrace(XZSTC_WARNING
 		   ,"Envoi message XDM_ECMD_Contraste non effectue \n");
    
		/*A Trace sortie fonction XZEP01_Commande_Contraste avec XDC_NOK */   

                XZST_03EcritureTrace(XZSTC_INTERFACE
                   ,"OUT: XZEP01_Commande_PICTO avec XDC_NOK");
 	
		return ( XDC_NOK );
         				
	}
   
   /*A Envoi du message */
   
   TipcSrvFlush();
   
   /*A Trace envoi message XDM_ECMD_Contraste effectue avec succes */    	
      	
   XZST_03EcritureTrace(XZSTC_INFO
 	   ,"Envoi message XDM_ECMD_Contraste effectue avec succes \n");
   
   /*A 	Trace sortie fonction XZEP01_Commande_Contraste avec XDC_OK */
   
   XZST_03EcritureTrace(XZSTC_INTERFACE
      ,"OUT: XZEP01_Commande_Contraste avec XDC_OK");
 	
  return ( XDC_OK );   

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Met en ou hors service un equipement externe connecte a
*  une machine sur un site
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC03_Mise_EnHorsSrv_Eqt (		XDY_NomSite	pa_NomSite,
					XDY_NomMachine	pa_NomMachine,
					int		va_TypeEquipement,
					int		va_NumEquipement,
					int		va_EnHorsService )		

/*
* ARGUMENTS EN ENTREE :
*   pa_NomSite		nom du site
*   pa_NomMachine	nom de la machine
*   pa_TypeEquipement	type d'equipt a mettre En ou Hors service
*   va_NumEquipement	numero de l'equipement a mettre En ou Hors service
*   va_EnHorsService	type d'action sur l'equipement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*   XZECC_ERR_ARG	argument va_EnHorsService invalide
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Construction du message XDM_ECMD_EQPT en specifiant le type 
*   d'action et en fournissant les valeurs des arguments recus.
*   Envoi du message sur le DataGroup XDG_ECMD_<TypeEquipement>_<NomMachine>.
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC03_Mise_EnHorsSrv_Eqt " ;

XDY_Datagroup	pl_NomDG;
XDY_Datagroup	pl_NomDGtmp;

   /*A Trace des parametres en entree */
   XZST_03EcritureTrace (	XZSTC_FONCTION,
 				"IN: XZEC03_Mise_EnHorsSrv_Eqt avec %s %s %d %d %d",
 				pa_NomSite,
 				pa_NomMachine,
 				va_TypeEquipement,
 				va_NumEquipement,
 				va_EnHorsService);
 				
   /* Controle du parametre action EnHorsService */
   if ( ( va_EnHorsService != XDC_EQT_HORS_SRV ) &&
        (va_EnHorsService !=  XDC_EQT_EN_SRV) )
   {
     /* Argument EnHorsService invalide */
     return ( XZECC_ERR_ARG );
   }

   /* Construction du DatGroup XDG_ECMD_<TypeEquipement>_<NomMachine> */
   if ( XZEC_nom_eqt ( va_TypeEquipement , pl_NomDGtmp ) == XDC_NOK )
   {
     /* Argument equipement invalide */
     return ( XZECC_ERR_TYPE_EQT );
   }
 
   XDG_EncodeDG3 ( pl_NomDG, XDG_ECMD, pl_NomDGtmp, strtok ( pa_NomMachine, " " ) ) ;	
   
   /* Envoi du message XDM_ECMD_EQPT en fournissant les valeur recues */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_EQPT)
                          ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT4,va_EnHorsService
                          ,T_IPC_FT_STR,strtok ( pa_NomMachine, " " )
                          ,T_IPC_FT_INT4,va_TypeEquipement
                          ,T_IPC_FT_INT4,va_NumEquipement
                          ,NULL))
      	{
         	XZST_03EcritureTrace(XZSTC_WARNING
 		,"Envoi message XDM_ECMD_EQPT EnHorsService non effectue ." );
 		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC03_Mise_EnHorsSrv_Eqt avec XDC_NOK");
 		return ( XDC_NOK );
         				
	}
   TipcSrvFlush();
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC03_Mise_EnHorsSrv_Eqt avec XDC_OK");
   return ( XDC_OK );   

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Active ou desactive un equipement externe connecte a
*  une machine sur un site
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC04_Activ_Desactiv_Eqt (		XDY_NomSite	pa_NomSite,
					XDY_NomMachine	pa_NomMachine,
					int		va_TypeEquipement,
					int		va_NumEquipement,
					int		va_ActivDesactiv )		

/*
* ARGUMENTS EN ENTREE :
*   pa_NomSite		nom du site
*   pa_NomMachine	nom de la machine
*   va_TypeEquipement	type d'equipt a activer ou a desactiver
*   va_NumEquipement	numero de l'equipement a activer ou a desactiver
*   va_ActivDesactiv	type d'action sur l'equipement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*   XZECC_ERR_ARG	argument va_ActivDesactiv invalide
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Construction du message XDM_ECMD_EQPT en specifiant le type 
*   d'action et en fournissant les valeurs des arguments recus.
*   Envoi du message sur le DataGroup XDG_ECMD_<TypeEquipement>_<NomMachine>.
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC04_Activ_Desactiv_Eqt " ;

XDY_Datagroup	pl_NomDG;
XDY_Datagroup	pl_NomDGtmp;

   /*A Trace des parametres en entree */
   XZST_03EcritureTrace (	XZSTC_FONCTION,
 				"IN: XZEC04_Activ_Desactiv_Eqt avec %s %s %d %d %d",
 				pa_NomSite,
 				pa_NomMachine,
 				va_TypeEquipement,
 				va_NumEquipement,
 				va_ActivDesactiv );

   /* Controle du parametre action Active Desactive */
   if ( ( va_ActivDesactiv != XDC_EQT_ACTIF ) &&
	( va_ActivDesactiv != XDC_EQT_INACTIF ) )
   {
     /* Argument EnHorsService invalide */
     return ( XZECC_ERR_ARG );
   }

   /* Construction du DatGroup XDG_ECMD_<TypeEquipement>_<NomMachine> */
   if ( XZEC_nom_eqt ( va_TypeEquipement , pl_NomDGtmp ) == XDC_NOK )
   {
     /* Argument equipement invalide */
     return ( XZECC_ERR_TYPE_EQT );
   }
 
   XDG_EncodeDG3 ( pl_NomDG, XDG_ECMD, pl_NomDGtmp, strtok ( pa_NomMachine, " " ) ) ;	
      
   /* Envoi du message XDM_ECMD_EQPT en fournissant les valeur recues */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_EQPT)
                          ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT4,va_ActivDesactiv
                          ,T_IPC_FT_STR,strtok ( pa_NomMachine, " " )
                          ,T_IPC_FT_INT4,va_TypeEquipement
                          ,T_IPC_FT_INT4,va_NumEquipement
                          ,NULL))
      	{
         	XZST_03EcritureTrace(XZSTC_WARNING
 		,"Envoi message XDM_ECMD_EQPT ActivDesactiv non effectue ." );
 		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC04_Activ_Desactiv_Eqt avec XDC_NOK");
 		return ( XDC_NOK );
         				
	}
   TipcSrvFlush();
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC04_Activ_Desactiv_Eqt avec XDC_OK");
   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de demarrer ou d'arreter les taches temporaires liees
*  a un equipement externe.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC05_Cmd_TacheTmp_Eqt (		XDY_NomSite	pa_NomSite,
					XDY_NomMachine	pa_NomMachine,
					int		va_TypeEquipement,
					int		va_MarcheArret )		

/*
* ARGUMENTS EN ENTREE :
*   pa_NomSite		nom du site ( PC district ou LT )
*   pa_NomMachine	nom de la machine
*   va_TypeEquipement	type d'equipt a concerne (XZECC_TYPE_EQT_ )
*   va_MarcheArret	type d'action sur les taches associees a l'equipement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*   XZECC_ERR_ARG	argument va_MarcheArret invalide
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Construction du message XDM_ECMD_EQPT en specifiant le type 
*   d'action et en fournissant les valeurs des arguments recus.
*   Envoi du message sur le DataGroup XDG_ECTL_EQPT_<NomMachine> du site concerne.
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC05_Cmd_TacheTmp_Eqt " ;

XDY_Datagroup	pl_NomDG;

   /*A Trace des parametres en entree */
   XZST_03EcritureTrace (	XZSTC_FONCTION,
 				"IN: XZEC05_Cmd_TacheTmp_Eqt avec %s %s %d %d %d",
 				pa_NomSite,
 				pa_NomMachine,
 				va_TypeEquipement,
 				va_MarcheArret );
 				

   /* Controle du parametre action Active Desactive */
   if ( ( va_MarcheArret != XZECC_EQT_MARCHE ) &&
	( va_MarcheArret != XZECC_EQT_ARRET ) )
   {
     /* Argument EnHorsService invalide */
     return ( XZECC_ERR_ARG );
   }


   /* Construction du DatGroup XDG_ECTL_EQPT_<NomMachine> */
   XDG_EncodeDG2 ( pl_NomDG, XDG_ECTL_EQPT, strtok ( pa_NomMachine, " " ) ) ;	

   /* Envoi du message XDM_ECMD_EQPT en fournissant les valeur recues */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_EQPT)
                          ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT4,va_MarcheArret
                          ,T_IPC_FT_STR,strtok ( pa_NomMachine, " " )
                          ,T_IPC_FT_INT4,va_TypeEquipement
                          ,T_IPC_FT_INT4,XDC_OK
                          ,NULL))
   {
      XZST_03EcritureTrace(XZSTC_WARNING
      ,"Envoi message XDM_ECMD_EQPT MarcheArret non effectue ." );
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC05_Cmd_TacheTmp_Eqt avec XDC_NOK");
      return ( XDC_NOK );
         				
   }
   TipcSrvFlush();
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC05_Cmd_TacheTmp_Eqt avec XDC_OK");
   return ( XDC_OK );   

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Inhibe ou a desinhibe un equipement externe connecte a
*  une machine sur un site
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC06_Inhibition_Eqt (		XDY_NomSite	pa_NomSite,
					XDY_NomMachine	pa_NomMachine,
					int		va_TypeEquipement,
					int		va_NumEquipement,
					int		va_EtatInhibition )		

/*
* ARGUMENTS EN ENTREE :
*   pa_NomSite		nom du site
*   pa_NomMachine	nom de la machine
*   va_TypeEquipement	type d'equipt a inhibe ou a desinhibe
*   va_NumEquipement	numero de l'equipement a inhibe ou a desinhibe
*   va_EtatInhibition	type d'action sur l'equipement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*   XZECC_ERR_ARG	argument va_EtatInhibition invalide
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Construction du message XDM_ECMD_EQPT en specifiant le type 
*   d'action et en fournissant les valeurs des arguments recus.
*   Envoi du message sur le DataGroup XDG_ECMD_<TypeEquipement>_<NomMachine>.
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC06_Inhibition_Eqt " ;

XDY_Datagroup	pl_NomDG;
XDY_Datagroup	pl_NomDGtmp;

   /*A Trace des parametres en entree */
   XZST_03EcritureTrace (	XZSTC_FONCTION,
 				"IN: XZEC06_Inhibition_Eqt avec %s %s %d %d %d",
 				pa_NomSite,
 				pa_NomMachine,
 				va_TypeEquipement,
 				va_NumEquipement,
 				va_EtatInhibition );

   /* Controle du parametre action Active Desactive */
   if ( ( va_EtatInhibition != XDC_EQT_ETAT_INHIBE ) &&
	( va_EtatInhibition != XDC_EQT_ETAT_NON_INHIBE ) )
   {
     /* Argument EnHorsService invalide */
     return ( XZECC_ERR_ARG );
   }

   /* Construction du DatGroup XDG_ECMD_<TypeEquipement>_<NomMachine> */
   if ( XZEC_nom_eqt ( va_TypeEquipement , pl_NomDGtmp ) == XDC_NOK )
   {
     /* Argument equipement invalide */
     return ( XZECC_ERR_TYPE_EQT );
   }
 
   XDG_EncodeDG3 ( pl_NomDG, XDG_ECMD, pl_NomDGtmp, strtok ( pa_NomMachine, " " ) ) ;	
      
   /* Envoi du message XDM_ECMD_EQPT en fournissant les valeur recues */
   if (!TipcSrvMsgWrite (  pl_NomDG
      			  ,XDM_IdentMsg(XDM_ECMD_EQPT)
                          ,XDM_FLG_SRVMSGWRITE
                          ,T_IPC_FT_INT4,va_EtatInhibition
                          ,T_IPC_FT_STR,strtok ( pa_NomMachine, " " )
                          ,T_IPC_FT_INT4,va_TypeEquipement
                          ,T_IPC_FT_INT4,va_NumEquipement
                          ,NULL))
      	{
         	XZST_03EcritureTrace(XZSTC_WARNING
 		,"Envoi message XDM_ECMD_EQPT ActivDesactiv non effectue ." );
 		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC06_Inhibition_Eqt avec XDC_NOK");
 		return ( XDC_NOK );
         				
	}
   TipcSrvFlush();
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: XZEC06_Inhibition_Eqt avec XDC_OK");
   return ( XDC_OK );   

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoyer l'etat des alarmes des equipements.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEC07_Alarme_Eqt (XDY_TypeEqt va_TypeEqt, XDY_Eqt va_NumEqt, XDY_Etat_Eqt va_Etat_Eqt)

/*
* ARGUMENTS EN ENTREE :
* XDY_TypeEqt	Type de l'≈quipement » rafra—chir
* XDY_Eqt	Num≈ro de l'≈quipement,
* XDY_Etat_Eqt	Etat de l'≈quipement.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex≈cution de la primitive
* XDY_OK		Ex≈cution correcte de la primitive
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr¬le des param…tres en entr≈e.
* Construction du message XDM_Etat_Dispo_Eqt et envoi dans le datagroup
* XDG_Etat_Dispo_Eqt
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC07_Alarme_Eqt" ;
int vl_code_retour = XDC_OK;

XZST_03EcritureTrace(XZSTC_INTERFACE , "IN : XZEC07_Alarme_Eqt: va_TypeEqt:%d, va_NumEqt:%d, va_Etat_Eqt:%d", va_TypeEqt , va_NumEqt , va_Etat_Eqt );
/*A Controle des Parametres*/

/*A Construction et envoi du Message */
if ( va_TypeEqt != XDC_EQT_PAU )
   {
   return XDC_OK;
   }  
         
if (!TipcSrvMsgWrite(   XDG_ECMD_RAU
                        ,XDM_IdentMsg(XDM_IETAT_DISPO_EQT) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT2,(T_INT2) va_TypeEqt
			,T_IPC_FT_INT2,(T_INT2) va_NumEqt
			,T_IPC_FT_INT4,(T_INT4) va_Etat_Eqt,
			NULL) )
   {
   /* Affichage Trace */
   XZST_03EcritureTrace(XZSTC_WARNING
                        ,"XZEC07_Alarme_Eqt:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,%d,%d,%d,NULL)" 
			,XDG_ECMD_RAU,XDM_IdentMsg(XDM_IETAT_DISPO_EQT), va_TypeEqt , va_NumEqt , va_Etat_Eqt );
   vl_code_retour = XDC_NOK;
   }

XZST_03EcritureTrace(XZSTC_INTERFACE , "OUT : XZEC07_Alarme_Eqt: %d " , vl_code_retour);
return vl_code_retour;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier contenant les coefficients de calcul des donnees 6 minutes glissantes.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC20_Verif_CDMG (	XDY_Fichier	va_Fd,		
			XDY_Entier *	va_Resultat )		

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd	: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC20_Verif_CDMG " ;

XDY_Texte		vl_Lig;
XDY_Texte		vl_Texte_out;
XDY_Entier		vl_num_ligne = 0;
int			vl_NbCarLu   = 0;
XDY_Entier		vl_Nb_Param = 0;
XDY_Entier		vl_retcode = 0;
XDY_Entier		vl_Resultat_out = 0;
char			vl_a[XZECC_TAILLE_TAMPON];
char			vl_b[XZECC_TAILLE_TAMPON];
char			vl_c[XZECC_TAILLE_TAMPON];
char			vl_d[XZECC_TAILLE_TAMPON];
char			vl_e[XZECC_TAILLE_TAMPON];
char			vl_f[XZECC_TAILLE_TAMPON];
XDY_Coeff 	 	vl_Somme_Coeff = 0.0;

   XZST_03EcritureTrace(XZSTC_FONCTION," IN  : XZEC20_Verif_CDMG File: %d", va_Fd );
   /* Test sur le file descriptor passe en parametre d'entree */
 
   if (va_Fd < 0)
   {
      XZST_03EcritureTrace(XZSTC_MESSAGE,"xzec20: fichier de classe DIV non existant");
      XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC20_Verif_CDMG Retour: XDC_NOK" );
      return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant » va_Fd 
   */
 
   while ( ( vl_NbCarLu = XZEC_LireLigneFich ( va_Fd, vl_Lig, sizeof(vl_Lig) ) ) != XZEXC_EOF )
   {
       /*A
       ** Comptage des lignes lues du fichier va_Fd 
       */
    
       vl_num_ligne++; 
          
       vl_retcode = strcspn( vl_Lig, XZECC_COMMENT );

       if (vl_Resultat_out != XZECC_EOF )
       {
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */

          *va_Resultat = vl_num_ligne;

          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presenece du caractere #
          */

          vl_retcode = strcspn ( vl_Lig, XZECC_COMMENT );

          if (vl_retcode != 0)
          {  
             /*B
             ** Extraction des differents champs de vl_Lig 
             */
             
             if ( ( vl_Nb_Param = sscanf ( vl_Lig, "%s\t%s\t%s\t%s\t%s\t%s", vl_a, vl_b, vl_c, vl_d, vl_e, vl_f ) ) != 6 )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"Nombre de valeurs lues non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec20 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
               
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_a, XZECC_LONG_COEFF, XZECC_COEFF ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"COEFF1 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec20 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_b, XZECC_LONG_COEFF, XZECC_COEFF ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"COEFF2 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec20 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_c, XZECC_LONG_COEFF, XZECC_COEFF ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"COEFF3 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec20 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_d, XZECC_LONG_COEFF, XZECC_COEFF ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"COEFF4 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec20 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_e, XZECC_LONG_COEFF, XZECC_COEFF ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"COEFF5 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec20 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_f, XZECC_LONG_COEFF, XZECC_COEFF ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"DIV5 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec20 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_Somme_Coeff = atof ( vl_a ) + atof ( vl_b ) + atof ( vl_c ) + atof ( vl_d ) + atof ( vl_e ) + atof ( vl_f ) ) != XZECC_MAX_COEFF )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"Somme des coefficients donnees glissantes > 1");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec20 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
               
             }
             
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier va_Fd 
       ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
       */
       
       {  
          if ( *va_Resultat == 0 )
          {
             XZST_03EcritureTrace(XZSTC_WARNING,"Fichier vide");
             return ( XDC_NOK );
          }
          else
          {
             *va_Resultat = 0;
             return ( XDC_OK );
          }
       }
         
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier va_Fd 
   ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
   */
    
   *va_Resultat = 0;
   XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC20_Verif_CDMG Retour: XDC_OK Result:%d", *va_Resultat );
   return ( XDC_OK );
   
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier de configuration des donnees classe de Distances Inter Vehiculaire.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC21_Verif_Classe_DIV (	XDY_Fichier	va_Fd,
				XDY_Entier *	va_Resultat )		

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd	: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC21_Verif_Classe_DIV " ;

XDY_Texte		vl_Lig;
XDY_Texte		vl_Texte_out;
XDY_Entier		vl_num_ligne = 0;
int			vl_NbCarLu   = 0;
XDY_Entier		vl_Nb_Param = 0;
XDY_Entier		vl_retcode = 0;
XDY_Entier		vl_Resultat_out = 0;
char			vl_DIV1[XZECC_TAILLE_TAMPON];
char			vl_DIV2[XZECC_TAILLE_TAMPON];
char			vl_DIV3[XZECC_TAILLE_TAMPON];
char			vl_DIV4[XZECC_TAILLE_TAMPON];
char			vl_DIV5[XZECC_TAILLE_TAMPON];
 

   XZST_03EcritureTrace(XZSTC_FONCTION," IN  : XZEC21_Verif_Classe_DIV File: %d", va_Fd );
   /* Test sur le file descriptor passe en parametre d'entree */
 
   if (va_Fd < 0)
   {
      XZST_03EcritureTrace(XZSTC_MESSAGE,"xzec21: fichier de classe DIV non existant");
      XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC21_Verif_Classe_DIV Retour: XDC_NOK" );
      return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant » va_Fd 
   */
 
   while ( ( vl_NbCarLu = XZEC_LireLigneFich ( va_Fd, vl_Lig, sizeof(vl_Lig) ) ) != XZEXC_EOF )
   {
       /*A
       ** Comptage des lignes lues du fichier va_Fd 
       */
    
       vl_num_ligne++; 
          
       vl_retcode = strcspn( vl_Lig, XZECC_COMMENT );

       if (vl_Resultat_out != XZECC_EOF )
       {
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */

          *va_Resultat = vl_num_ligne;

          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presenece du caractere #
          */

          vl_retcode = strcspn ( vl_Lig, XZECC_COMMENT );

          if (vl_retcode != 0)
          {  
             /*B
             ** Extraction des differents champs de vl_District 
             */
             
             if ( ( vl_Nb_Param = sscanf ( vl_Lig, "%s\t%s\t%s\t%s\t%s", vl_DIV1, vl_DIV2, vl_DIV3, vl_DIV4, vl_DIV5 ) ) != 5 )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"Nombre de valeurs lues non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec21 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
               
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_DIV1, XZECC_LONG_DIV, XZECC_DIV ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"DIV1 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec21 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_DIV2, XZECC_LONG_DIV, XZECC_DIV ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"DIV2 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec21 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_DIV3, XZECC_LONG_DIV, XZECC_DIV ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"DIV3 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec21 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_DIV4, XZECC_LONG_DIV, XZECC_DIV ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"DIV4 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec21 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_DIV5, XZECC_LONG_DIV, XZECC_DIV ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"DIV5 invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec21 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             
             
             
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier va_Fd 
       ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
       */
       
       {  
          if ( *va_Resultat == 0 )
          {
             XZST_03EcritureTrace(XZSTC_WARNING,"Fichier vide");
             return ( XDC_NOK );
          }
          else
          {
             *va_Resultat = 0;
             return ( XDC_OK );
          }
       }
         
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier va_Fd 
   ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
   */
    
   *va_Resultat = 0;
   XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC21_Verif_Classe_DIV Retour: XDC_OK Result:%d", *va_Resultat );
   return ( XDC_OK );
   
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier contenant des seuils trafic pour alerte DAI.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC22_Seuil_Trafic (	XDY_Fichier	va_Fd,	
				XDY_Entier *	va_Resultat )		

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd	: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC22_Seuil_Trafic " ;

XDY_Texte		vl_Lig;
XDY_Texte		vl_Texte_out;
XDY_Entier		vl_num_ligne = 0;
int			vl_NbCarLu   = 0;
XDY_Entier		vl_Nb_Param = 0;
XDY_Entier		vl_retcode = 0;
XDY_Entier		vl_Resultat_out = 0;
char			vl_Sens[XZECC_TAILLE_TAMPON];
char			vl_Contexte[XZECC_TAILLE_TAMPON];
char			vl_TO_f_ps[XZECC_TAILLE_TAMPON];
char			vl_TO_ps_s[XZECC_TAILLE_TAMPON];
char			vl_TO_s_ps[XZECC_TAILLE_TAMPON];
char			vl_TO_ps_f[XZECC_TAILLE_TAMPON];
char			vl_V_f_ps[XZECC_TAILLE_TAMPON];
char			vl_V_ps_s[XZECC_TAILLE_TAMPON];
char			vl_V_s_ps[XZECC_TAILLE_TAMPON];
char			vl_V_ps_f[XZECC_TAILLE_TAMPON];
char			vl_Q_f_s[XZECC_TAILLE_TAMPON];
char			vl_Q_s_f[XZECC_TAILLE_TAMPON];
 

   XZST_03EcritureTrace(XZSTC_FONCTION," IN  : XZEC22_Seuil_Trafic File: %d", va_Fd );
   /* Test sur le file descriptor passe en parametre d'entree */
 
   if (va_Fd < 0)
   {
      XZST_03EcritureTrace(XZSTC_MESSAGE,"xzec22: fichier de classe DIV non existant");
      XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC22_Seuil_Trafic Retour: XDC_NOK" );
      return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant » va_Fd 
   */
 
   while ( ( vl_NbCarLu = XZEC_LireLigneFich ( va_Fd, vl_Lig, sizeof(vl_Lig) ) ) != XZEXC_EOF )
   {
       /*A
       ** Comptage des lignes lues du fichier va_Fd 
       */
    
       vl_num_ligne++; 
          
       vl_retcode = strcspn( vl_Lig, XZECC_COMMENT );

       if (vl_Resultat_out != XZECC_EOF )
       {
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */

          *va_Resultat = vl_num_ligne;

          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presenece du caractere #
          */

          vl_retcode = strcspn ( vl_Lig, XZECC_COMMENT );

          if (vl_retcode != 0)
          {  
             /*B
             ** Extraction des differents champs de vl_District 
             */
             
             if ( ( vl_Nb_Param = sscanf ( vl_Lig, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s", 
                                        vl_Sens, vl_Contexte, 
                                        vl_TO_f_ps, vl_TO_ps_s, vl_TO_s_ps, vl_TO_ps_f,
                                        vl_V_f_ps, vl_V_ps_s, vl_V_s_ps, vl_V_ps_f,
                                        vl_Q_f_s, vl_Q_s_f ) ) != 12 )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"Nombre de valeurs lues non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
               
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_TO_f_ps, XZECC_LONG_SEUIL_TO, XZECC_SEUIL_TO ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_TO f_ps invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_TO_ps_s, XZECC_LONG_SEUIL_TO, XZECC_SEUIL_TO ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_TO ps_s invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_TO_s_ps, XZECC_LONG_SEUIL_TO, XZECC_SEUIL_TO ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_TO s_ps invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_TO_ps_f, XZECC_LONG_SEUIL_TO, XZECC_SEUIL_TO ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_TO ps_f invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_V_f_ps, XZECC_LONG_SEUIL_V, XZECC_SEUIL_V ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_V f_ps invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_V_ps_s, XZECC_LONG_SEUIL_V, XZECC_SEUIL_V ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_V ps_s invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_V_s_ps, XZECC_LONG_SEUIL_V, XZECC_SEUIL_V ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_V s_ps invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_V_ps_f, XZECC_LONG_SEUIL_V, XZECC_SEUIL_V ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_V ps_f invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_Q_f_s, XZECC_LONG_SEUIL_Q, XZECC_SEUIL_Q ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_Q f_s invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             if ( ( vl_retcode = XZEC00_Verif_Coherence_Nombre ( vl_Q_s_f, XZECC_LONG_SEUIL_Q, XZECC_SEUIL_Q ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"SEUIL_Q s_f invalide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzec22 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             
             
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier va_Fd 
       ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
       */
       
       {  
           if ( *va_Resultat == 0 )
          {
             XZST_03EcritureTrace(XZSTC_WARNING,"Fichier vide");
             return ( XDC_NOK );
          }
          else
          {
             *va_Resultat = 0;
             return ( XDC_OK );
          }
       }
         
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier va_Fd 
   ** Retour du status XDC_OK et du parametre *va_Resultat = NULL 
   */
    
   *va_Resultat = 0;
   XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC22_Seuil_Trafic Retour: XDC_OK Result:%d", *va_Resultat );
   return ( XDC_OK );
   
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier de configuration des scenarios NAV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC23_Scenario_Nav (	XDY_Fichier	va_Fd,	
				XDY_Entier *	va_ResultatOut )		

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd	: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC23_Scenario_Nav " ;

XDY_Texte	pl_LigneFichier;
XDY_Entier	vl_num_ligne = 0;
int		vl_NbCarLu   = 0;
 
   XZST_03EcritureTrace(XZSTC_FONCTION," IN  : XZEC23_Scenario_Nav File: %d", va_Fd );

   /* Test sur le file descriptor passe en parametre d'entree */
 
   if (va_Fd < 0)
   {
      XZST_03EcritureTrace(XZSTC_MESSAGE,"xzec20: fichier de coef 6 mn glissante non existant");
      XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC23_Scenario_Nav Retour: XDC_NOK" );
      return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant » va_Fd 
   */
 
   while ( ( vl_NbCarLu = XZEC_LireLigneFich ( va_Fd, pl_LigneFichier, sizeof(pl_LigneFichier) ) ) != XZEXC_EOF )
   {
       /*A
       ** Comptage des lignes lues du fichier va_Fd 
       */
    
       vl_num_ligne++;
   }
   
   /*! Attention ceci est une bidouille pour coquille vide */
   if ( vl_num_ligne == 0 ) 
   {
      *va_ResultatOut = 1;
      XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC23_Scenario_Nav Retour: XDC_NOK Result: %d", *va_ResultatOut );
      return ( XDC_NOK );
   }
   
   *va_ResultatOut = 0;
   XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC23_Scenario_Nav Retour: XDC_OK Result: %d", *va_ResultatOut );
   return ( XDC_OK );
   
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier de configuration des equations d'alerte.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEC24_Equation_Alerte (	XDY_Fichier	va_Fd,	
				XDY_Entier *	va_ResultatOut )		

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd		: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/
{
static char *version = "@(#)xzec.c	1.14 04/07/97 : XZEC24_Equation_Alerte " ;

XDY_Texte	pl_LigneFichier;
XDY_Entier	vl_num_ligne = 0;
int		vl_NbCarLu   = 0;
 

   XZST_03EcritureTrace(XZSTC_FONCTION," IN  : XZEC24_Equation_Alerte File: %d", va_Fd );
   /*A Test sur le file descriptor passe en parametre d'entree */
 
   if (va_Fd < 0)
   {
      XZST_03EcritureTrace(XZSTC_MESSAGE,"xzec20: fichier de coef 6 mn glissante non existant");
      *va_ResultatOut = 0;
      XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC24_Equation_Alerte Retour: XDC_NOK Result: %d", *va_ResultatOut );
      return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant » va_Fd 
   */
 
   while ( ( vl_NbCarLu = XZEC_LireLigneFich ( va_Fd, pl_LigneFichier, sizeof(pl_LigneFichier) ) ) != XZEXC_EOF )
   {
       /*A
       ** Comptage des lignes lues du fichier va_Fd 
       */
    
       vl_num_ligne++;
   }
   
   /*! Attention ceci est une bidouille pour coquille vide */
   if ( vl_num_ligne == 0 ) 
   {
      *va_ResultatOut = 1;
      XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC24_Equation_Alerte Retour: XDC_NOK Result: %d", *va_ResultatOut );
      return ( XDC_NOK );
   }
   
   
   *va_ResultatOut = 0;
   XZST_03EcritureTrace(XZSTC_FONCTION," OUT : XZEC24_Equation_Alerte Retour: XDC_OK Result: %d", *va_ResultatOut );
   return ( XDC_OK );
   
}
