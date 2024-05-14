/*E*/
/* Fichier : $Id: xzao493.c,v 1.4 2021/06/07 10:16:39 pc2dpdy Exp $        $Revision: 1.4 $        $Date: 2021/06/07 10:16:39 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao493.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module des objets statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	28/07/20	: Creation IMU 1.1 DEM-SAE155
* ABE	07/06/21	: Fix NomMachine DEM-SAE155 1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"
#include "xzao493.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzao493.c	1.1 1.1 28/07/20" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*    Lit le fichier de configuration des IMU et
*    appelle la fonction utilisateur de GECONF
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int xzao493_Lire_Config_IMU ( 	FILE  		*pa_Fd,
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
*       XZAOT_ConfIMU 	contenu de ligne
*       XDY_Entier 		NumLigne
*    La fonction utilisateur retourne XDC_OK ou XDC_NOK . arret code de la lecture
*    et code de retour XDC_NOK
*
* MECANISMES :
*
* UNIX28
------------------------------------------------------*/

{
   static char *version = "@(#)xzao493.c	1.1 28/07/20 : xzao493_Lire_Config_IMU" ;
   
   XZAOT_ConfIMU	vl_IMU;
   char			vl_Lig[263];
   char			*vl_Texte;
   char			vl_Texte_out[263];
   XDY_Entier 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   XDY_Entier		vl_Resultat_out;
 
   /*A
   ** Test sur le file descriptor passe en parametre d'entree 
   */
 
   if (pa_Fd == NULL)
   {
     XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao493: fichier non existant");
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
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Nom IMU non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_IMU.NomIMU, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %s",vl_IMU.NomIMU); 


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );            
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte, XDC_Lon_NomAuto, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Autoroute non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_IMU.Autoroute, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %s",vl_IMU.Autoroute); 


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );            
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_LONG, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie PR non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_IMU.PR = atoi ( vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %d",vl_IMU.PR); 


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_UCHAR, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Sens non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_IMU.Sens = atoi ( vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %d",vl_IMU.Sens);


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_Texte, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie AdresseEndPointOPCUA  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_IMU.AdresseEndPointOPCUA, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %s",vl_IMU.AdresseEndPointOPCUA); 


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_Texte, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie UserOPCUA  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_IMU.UserOPCUA, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %s",vl_IMU.UserOPCUA); 


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_Texte, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie PasswordOPCUA  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_IMU.PasswordOPCUA, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %s",vl_IMU.PasswordOPCUA); 


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Nom Serveur  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
	     strcpy ( vl_IMU.NomServeur, vl_Texte_out );
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %s",vl_IMU.NomServeur);


	     vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
	     if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,   XDC_Lon_NomEqt, vl_Texte_out ) ) != XDC_OK )
	     {
	        XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Nom BICE  non valide");
		XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
		return ( XDC_NOK );
	     }
             strcpy ( vl_IMU.NomBICE, vl_Texte_out );
            XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %s",vl_IMU.NomBICE); 


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_NomSite, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Nom Site  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_IMU.NomSite, vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %s",vl_IMU.NomSite);


             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_BIT, vl_Texte_out ) ) != XDC_OK )
             {
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Supprime non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_IMU.Supprime = atoi ( vl_Texte_out );
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %d",vl_IMU.Supprime);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );            
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Port Maitre non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_IMU.PortMaitre = atoi ( vl_Texte_out );
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %d",vl_IMU.PortMaitre);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );            
             if ( ( vl_retcode = XZAO000_Verif_Coherence_Nombre ( vl_Texte, XZAO_LONG_INT, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Sous Type non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             vl_IMU.SousType = atoi ( vl_Texte_out );
	     XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %d",vl_IMU.SousType);

             vl_Texte = strtok ( NULL, XZAO_TAB_SEPAR );
             if ( ( vl_retcode = XZAO000_Verif_Coherence_String ( vl_Texte,  XDC_Lon_NomMachine, vl_Texte_out ) ) != XDC_OK )
             {  
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : Saisie Nom Machine  non valide");
                XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : retourne %d",XDC_NOK);
                return ( XDC_NOK );
             }
             strcpy ( vl_IMU.NomMachine, vl_Texte_out );
             XZST_03EcritureTrace(XZSTC_WARNING,"xzao493 : %s",vl_IMU.NomMachine);


             /*A
             ** Verification que la fonction utilisateur passee en parametre est non NULL 
             */
    
             if (pa_FonctionUtil != NULL)
             {
                /*B
                ** Appel de la fonction utilisateur passee en parametre 
                */
         
                if ( (vl_retcode = ( *pa_FonctionUtil ) ( vl_IMU, vl_num_ligne ) ) != XDC_OK )
                {    
                   XZST_03EcritureTrace(XZSTC_WARNING,"xzao493: appel de FonctionUtilisateur s'est mal deroule");
                   return ( XDC_NOK );
                }  
	     } 
             else
             {
                /*B
                ** Fonction utilisateur inexistante =>
                ** Retour du status XDC_NOK et du numero de ligne 
                */

                XZST_03EcritureTrace(XZSTC_MESSAGE,"xzao493: echec de l'appel de la fonction utilisateur");
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

/* Fin de la fonction xzao493_Lire_Config_IMU */

