/*E*/
/* Fichier : $Id: xzao103.c,v 1.7 2019/01/24 16:49:50 devgfi Exp $        $Revision: 1.7 $        $Date: 2019/01/24 16:49:50 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao103.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	18 Oct 1994	: Creation
* volcic	28 Oct 1994	: Modification de recuperation des parametres de
*				  la structure (v1.2)
* JMG   31/10/07        : ajout site de gestion 1.3
* JPL	13/07/16 : Concordance types d'arguments pour ia64 : fonction utilisateur : numero ligne XDY_Entier  1.4
* LCL	03/09/18 : Suppression SAGA DEM1306 1.5
* LCL	06/11/18 : Ajouts type LCR et id LCR DEM1306 1.6
* JPL	24/01/19 : Lecture de l'indicateur 'Supprime' en configuration (DEM 1305)  1.7
* GGY 21/04/23 : Modification du type de variable pour le port XZAO_LONG_LONG (DEM-472)
* GGY	26/04/23	:	Ajout pannonceau (DEM-473)
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzao103.c,v 1.7 2019/01/24 16:49:50 devgfi Exp $ : xzao103" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des PMV Picto et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao103_Lire_Config_PMV_Picto ( 	FILE  		*pa_Fd,
					XDY_FonctionInt pa_FonctionUtil,
					XDY_Entier   	*pa_Resultat ) 
				        
/*
* ARGUMENTS EN ENTREE :
*
* FILE			pa_Fd
* XDY_FonctionInt	pa_FonctionUtil
*
* ARGUMENTS EN SORTIE :	
*
* XDY_Entier		*pa_Resultat:OK=NULL ou ligne de l'erreur du systeme
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
*    Appelle la fonction utilisateur avec les arguments:
*       XZAOT_ConfPMV 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "$Id: xzao103.c,v 1.7 2019/01/24 16:49:50 devgfi Exp $ : xzao103_Lire_Config_PMV" ;
   
   XZAOT_ConfPMV	vl_PMV;
   char			vl_Lig[255];
   char			*vl_Texte;
   char			vl_Texte_out[255];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao103: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant � pa_Fd
   */
    
   while ( ( vl_retcode = XZAO000_LireLigneFich ( pa_Fd, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
   {
       /*A
       ** Comptage des lignes lues du fichier pa_Fd 
       */
    
       vl_num_ligne++; 
       
       if (vl_Resultat_out != XZAO_EOF )
       { 
          /*A
          ** Affectation du parametre de sortie egal au numero de la ligne lue
          */
          
          *pa_Resultat = vl_num_ligne;
             
          /*A
          ** Verification que la ligne n'est pas un commentaire
          ** Presence du caractere #
          */
          
          vl_retcode = strcspn ( vl_Lig, XZAO_COMMENT );

          if (vl_retcode != XDC_OK) 

          {
             /*A
             ** Extraction des differents champs de vl_District
             */
    
             vl_Texte = strtok ( vl_Lig, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMV.NomPMV, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMV.Autoroute, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PMV.PR = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PMV.Sens = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMV.NomServeur, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PMV.LiaisonPMV = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Rgs, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMV.RgsPMV, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMV.TypePMV, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PMV.LiaisonPicto = atoi ( vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Rgs, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMV.RgsPicto, vl_Texte_out );
              
             
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMV.TypePicto, vl_Texte_out );
              
   		/*2009*/          
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_Nom, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMV.NomSite, vl_Texte_out );
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_AdresseIP, vl_Texte_out ) ) != XDC_OK )
             { 
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie AdresseIP  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_PMV.AdresseIP, vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             { 
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Saisie Port IP Maitre non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_PMV.PortIP = atol ( vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Type LCR non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             vl_PMV.TypeLCR = atoi ( vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, 20, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : ID LCR non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             
             strcpy ( vl_PMV.IdLCR, vl_Texte_out );

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );

             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : Indicateur suppression non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao103 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }

             vl_PMV.Supprime = atoi ( vl_Texte_out );

             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_PMV, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao103: appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao103: echec de l'appel de la fonction utilisateur");
                return ( XDC_NOK );
             }
          }
       }
       
       else
       
       /* A
       ** Fin de lecture du fichier pa_Fd 
       ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
       */
       
       {  
          *pa_Resultat = NULL;
          return ( XDC_OK );
       }
   }
 
   /*A
   ** Fin de la boucle de lecture du fichier pa_Fd 
   ** Retour du status XDC_OK et du parametre *pa_Resultat = NULL 
   */
   
*pa_Resultat = NULL;

return XDC_OK;

}   

/* Fin de la fonction xzao103_Lire_Config_PMV */

