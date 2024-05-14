/*E*/
/* Fichier : $Id: xzao259.c,v 1.2 2018/10/26 16:21:53 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/10/26 16:21:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao259.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  LCL	21/02/18	: Creation CTRL_FEUX 1.1 DEM1284
* JPL	26/10/18 : Lecture de l'indicateur 'Supprime' en configuration (DEM 1305)  1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao259.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzao259.c	1.1 1.1 21/02/18" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des CFE  et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao259_Lire_Config_CFE ( 	FILE  		*pa_Fd,
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
*       XZAOT_ConfCFE 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "@(#)xzao259.c	1.1 21/02/18 : xzao259_Lire_Config_CFE" ;
   
   XZAOT_ConfCFE	vl_CFE;
   char			vl_Lig[259];
   char			*vl_Texte;
   char			vl_Texte_out[259];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao259: fichier non existant");
     return ( XDC_NOK );
   }
 
   /*A
   ** Boucle effectuant la lecture ligne par ligne du fichier correspondant a pa_Fd
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
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : Saisie Nom CFE non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_CFE.NomCFE, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : %s",vl_CFE.NomCFE); 
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );            
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : Saisie Autoroute non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_CFE.Autoroute, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : %s",vl_CFE.Autoroute); 
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );            
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : Saisie PR non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_CFE.PR = atoi ( vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : %d",vl_CFE.PR); 
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : Saisie Sens non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_CFE.Sens = atoi ( vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : %d",vl_CFE.Sens); 
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : Saisie Nom Serveur  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_CFE.NomServeur, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : %s",vl_CFE.NomServeur); 
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );            
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_AdresseIP, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : Saisie Adresse IP non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_CFE.AdresseIP, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : %s",vl_CFE.AdresseIP); 
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );            
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : Saisie Port IP non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_CFE.Port = atoi ( vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : %d",vl_CFE.Port); 
             
             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_NomSite, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : Saisie Nom Site  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_CFE.NomSite, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : %s",vl_CFE.NomSite); 

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : Saisie Supprime non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao259 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_CFE.Supprime = atoi ( vl_Texte_out );

             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_CFE, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao259: appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao259: echec de l'appel de la fonction utilisateur");
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

/* Fin de la fonction xzao259_Lire_Config_CFE */

