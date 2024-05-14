/*E*/
/* Fichier : $Id: xzao203sp.prc,v 1.8 2018/11/06 09:43:35 devgfi Exp $        $Revision: 1.8 $        $Date: 2018/11/06 09:43:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao203sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Oct 1994	: Creation
* volcic	14 Nov 1994	: Modif retour liste Sequences (v1.2)
* mismer	24 Avr 1997	: Ajout colonne alerte + type gabarit,vitesse et niche (DEM/1421) (v1.5)
* JPL		17/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.8
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao203sp.h" */

/* static char *version = "@(#)xzao203.c	1.1 23/18/94 : xzao203" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Description de la GTC Nice pour TEINI
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;203
   	@va_Basedd_in        		char(3),
   	@va_NomMachine_in		char(10),
   	@va_Numero_out			smallint  = null  output,
	@va_NomServeur_out		char(10)  = null  output,
	@va_LongueurITOR_out		smallint  = null  output,
	@va_AdresseITOR_out		smallint  = null  output,
	@va_LongueurETOR_out		smallint  = null  output,
	@va_AdresseETOR_out		smallint  = null  output,
	@va_LongTelealarme_out		smallint  = null  output,
	@va_AdresseTelealarme_out	smallint  = null  output,
	@va_LongTelecomEch_out		smallint  = null  output,
	@va_AdresseTelecomEch_out	smallint  = null  output,
	@va_LongTelecomTub_out		smallint  = null  output,
	@va_AdresseTelecomTub_out	smallint  = null  output,
	@va_LongTelesignEch_out		smallint  = null  output,
	@va_AdresseTelesignEch_out	smallint  = null  output,
	@va_LongTelesignTub_out		smallint  = null  output,
	@va_AdresseTelesignTub_out	smallint  = null  output,
	@va_LongSeqEch_out		smallint  = null  output,
	@va_AdresseSeqEch_out		smallint  = null  output,
	@va_LongSeqTub_out		smallint  = null  output,
	@va_AdresseSeqTub_out		smallint  = null  output,
	@va_LongAlerteTub_out		smallint  = null  output,
	@va_AdresseAlerteTub_out	smallint  = null  output
   	as

/* 
*
* SP	XZAO203SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in		
* XDY_Machine	va_NomMachine_in		
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des GTC Nice
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
declare
   
   /*A
   ** Declaration des variables locales
   */
   
   @vl_Machine		smallint
    
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_ARG_INV  
   */
      
   if @va_Basedd_in = null
   or @va_NomMachine_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   **   Les parametres d'entree ne sont pas nuls  
   */
      
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche du numero de la machine
         */
         
         select @vl_Machine = numero from CFG..EQT_GEN 
           where type = XDC_EQT_MAC
             and nom  = @va_NomMachine_in
             
         /*A
         ** Affectation des parametres de sorties (XZAOT_GTC_NICE)    
         */
         
         select @va_Numero_out 			= NIC.numero, 
         	@va_NomServeur_out		= GNIC.nom,
		@va_LongueurITOR_out		= NIC.longueur_ITOR,
		@va_AdresseITOR_out		= NIC.adresse_ITOR,
		@va_LongueurETOR_out		= NIC.longueur_ETOR,
		@va_AdresseETOR_out		= NIC.adresse_ETOR,
		@va_LongTelealarme_out		= NIC.longueur_telealarme,
		@va_AdresseTelealarme_out	= NIC.adresse_telealarme,
		@va_LongTelecomEch_out		= NIC.longueur_telecom_ech,
		@va_AdresseTelecomEch_out	= NIC.adresse_telecom_ech,
		@va_LongTelecomTub_out		= NIC.longueur_telecom_tub,
		@va_AdresseTelecomTub_out	= NIC.adresse_telecom_tub,
		@va_LongTelesignEch_out		= NIC.longueur_telesign_ech,
		@va_AdresseTelesignEch_out	= NIC.adresse_telesign_ech,
		@va_LongTelesignTub_out		= NIC.longueur_telesign_tub,
		@va_AdresseTelesignTub_out	= NIC.adresse_telesign_tub,
		@va_LongSeqEch_out		= NIC.longueur_sequence_ech,
		@va_AdresseSeqEch_out		= NIC.adresse_sequence_ech,
		@va_LongSeqTub_out		= NIC.longueur_sequence_tub,
		@va_AdresseSeqTub_out		= NIC.adresse_sequence_tub,
		@va_LongAlerteTub_out		= NIC.longueur_alerte_tub,
		@va_AdresseAlerteTub_out	= NIC.adresse_alerte_tub

         	
         from CFG..EQT_GEN GNIC, CFG..EQT_NIC NIC
         
         where GNIC.type 	= XDC_EQT_NIC
           and GNIC.maitre	= @vl_Machine
           and GNIC.type_maitre	= XDC_MAI_NIC
           and GNIC.eqt_supprime = XDC_FAUX
           and NIC.numero	= GNIC.numero
         
         
         if @@rowcount != 0
         begin       
	 
	 /*A
         ** Liste des GTC Tunnels    
         */
         
         select TUB.numero, 
         	adresse_etat, 
         	adresse_commande, 
         	adresse_discordance,
         	adresse_extincteur,
         	adresse_lit,
         	adresse_OP_CO,
         	adresse_telealarme,
         	adresse_gabarit,
         	adresse_vitesse,
         	adresse_niche
         	
         from CFG..EQT_TUB TUB, CFG..EQT_GEN GEN

         where GEN.type = XDC_EQT_TUB
           and GEN.eqt_supprime = XDC_FAUX
           and TUB.numero = GEN.numero 
         
         order by numero
         
         /*A
         ** Liste des GTC Demi Echangeur    
         */
         
         select ECH.numero, 
         	adresse_etat, 
         	adresse_commande, 
         	adresse_discordance,
         	adresse_telealarme
         	
         from CFG..EQT_ECH ECH, CFG..EQT_GEN GEN

         where GEN.type = XDC_EQT_ECH
           and GEN.eqt_supprime = XDC_FAUX
           and ECH.numero = GEN.numero 
         
         order by numero
         
                  			
         /*A
         ** Liste des GTC Sequence Tunnel    
         */
         
         select tube, sequence, adresse, bit_comm, bit_disc from CFG..EQT_SQA_TUB
              
         /*A
         ** Liste des GTC Sequence Echangeur    
         */
         
         select echangeur, sequence, adresse, bit_comm, bit_disc from CFG..EQT_SQA_ECH
         
         /*A
         ** Liste des GTC Capteur Tunnel    
         */
         
         select distinct GEN.maitre,
         	CPT.numero, 
         	CPT.type, 
         	CPT.num_bit,
          	CPT.num_alerte 
        	
         from CFG..EQT_CPT CPT, CFG..EQT_GEN GEN
         
         where ( GEN.type	= XDC_EQT_LIT
	   or  GEN.type = XDC_EQT_EXT
	   or  GEN.type = XDC_EQT_OPA
	   or  GEN.type = XDC_EQT_CCO
	   or  GEN.type = XDC_EQT_GAB
	   or  GEN.type = XDC_EQT_VIT
	   or  GEN.type = XDC_EQT_NICHE )
           and GEN.eqt_supprime = XDC_FAUX
           and CPT.numero = GEN.numero 
         
         order by CPT.type, CPT.numero
         
         /*A
         ** Liste des GTC FAV Tunnel    
         */
         
         select GEN.maitre,
         	FAV.numero, 
         	FAV.voie, 
         	FAV.rang_comm, 
         	FAV.rang_comm2, 
         	FAV.rang_etat, 
         	FAV.rang_etat2, 
         	FAV.bit_disc, 
         	FAV.bit_disc2 
         	
         from CFG..EQT_FAV FAV, CFG..EQT_GEN GEN
         
         where GEN.type	  = XDC_EQT_FAV
           and GEN.eqt_supprime = XDC_FAUX
           and GEN.numero = FAV.numero
         
         order by FAV.numero
         
         /*A
         ** Liste des GTC Panneau    
         */
         
         select GEN.maitre,
         	GEN.type_maitre,
         	PAN.numero, 
         	PAN.rang_comm, 
         	PAN.rang_comm2, 
         	PAN.rang_etat, 
         	PAN.rang_etat2, 
         	PAN.bit_disc, 
         	PAN.bit_disc2 
         	
         from CFG..EQT_PAN PAN, CFG..EQT_GEN GEN
         
         where GEN.type	  = XDC_EQT_PAN
           and GEN.eqt_supprime = XDC_FAUX
           and PAN.numero = GEN.numero 
         
         order by PAN.numero
         
         end
         
      end
      
#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
      
         /*A
         ** Recherche du numero de la machine
         */
         
         select @vl_Machine = numero from CFT..EQT_GEN 
           where type = XDC_EQT_MAC
             and nom  = @va_NomMachine_in
             
         /*A
         ** Affectation des parametres de sorties (XZAOT_GTC_NICE)    
         */
         
         select @va_Numero_out 			= NIC.numero, 
         	@va_NomServeur_out		= GNIC.nom,
		@va_LongueurITOR_out		= NIC.longueur_ITOR,
		@va_AdresseITOR_out		= NIC.adresse_ITOR,
		@va_LongueurETOR_out		= NIC.longueur_ETOR,
		@va_AdresseETOR_out		= NIC.adresse_ETOR,
		@va_LongTelealarme_out		= NIC.longueur_telealarme,
		@va_AdresseTelealarme_out	= NIC.adresse_telealarme,
		@va_LongTelecomEch_out		= NIC.longueur_telecom_ech,
		@va_AdresseTelecomEch_out	= NIC.adresse_telecom_ech,
		@va_LongTelecomTub_out		= NIC.longueur_telecom_tub,
		@va_AdresseTelecomTub_out	= NIC.adresse_telecom_tub,
		@va_LongTelesignEch_out		= NIC.longueur_telesign_ech,
		@va_AdresseTelesignEch_out	= NIC.adresse_telesign_ech,
		@va_LongTelesignTub_out		= NIC.longueur_telesign_tub,
		@va_AdresseTelesignTub_out	= NIC.adresse_telesign_tub,
		@va_LongSeqEch_out		= NIC.longueur_sequence_ech,
		@va_AdresseSeqEch_out		= NIC.adresse_sequence_ech,
		@va_LongSeqTub_out		= NIC.longueur_sequence_tub,
		@va_AdresseSeqTub_out		= NIC.adresse_sequence_tub,
		@va_LongAlerteTub_out		= NIC.longueur_alerte_tub,
		@va_AdresseAlerteTub_out	= NIC.adresse_alerte_tub

         	
         from CFT..EQT_GEN GNIC, CFT..EQT_NIC NIC
         
         where GNIC.type 	= XDC_EQT_NIC
           and GNIC.maitre	= @vl_Machine
           and GNIC.type_maitre	= XDC_MAI_NIC
           and GNIC.eqt_supprime = XDC_FAUX
           and NIC.numero	= GNIC.numero
         
         
         
	 if @@rowcount != 0
	 begin
	 
	 /*A
         ** Liste des GTC Tunnels    
         */
         
         select TUB.numero, 
         	adresse_etat, 
         	adresse_commande, 
         	adresse_discordance,
         	adresse_extincteur,
         	adresse_lit,
         	adresse_OP_CO,
         	adresse_telealarme,
         	adresse_gabarit,
         	adresse_vitesse,
         	adresse_niche
         	
         from CFT..EQT_TUB TUB, CFT..EQT_GEN GEN

         where GEN.type = XDC_EQT_TUB
           and GEN.eqt_supprime = XDC_FAUX
           and TUB.numero = GEN.numero 
         
         order by numero
         
         /*A
         ** Liste des GTC Demi Echangeur    
         */
         
         select ECH.numero, 
         	adresse_etat, 
         	adresse_commande, 
         	adresse_discordance,
         	adresse_telealarme
         	
         from CFT..EQT_ECH ECH, CFT..EQT_GEN GEN

         where GEN.type = XDC_EQT_ECH
           and GEN.eqt_supprime = XDC_FAUX
           and ECH.numero = GEN.numero 
         
         order by numero
         
                  			
         /*A
         ** Liste des GTC Sequence Tunnel    
         */
         
         select tube, sequence, adresse, bit_comm, bit_disc from CFT..EQT_SQA_TUB
              
         /*A
         ** Liste des GTC Sequence Echangeur    
         */
         
         select echangeur, sequence, adresse, bit_comm, bit_disc from CFT..EQT_SQA_ECH
         
         /*A
         ** Liste des GTC Capteur Tunnel    
         */
         
         select GEN.maitre,
         	CPT.numero, 
         	CPT.type, 
         	CPT.num_bit,
         	CPT.num_alerte 
         	
         from CFT..EQT_CPT CPT, CFT..EQT_GEN GEN
         
         where GEN.type	  = XDC_EQT_CPT
           and GEN.eqt_supprime = XDC_FAUX
           and CPT.numero = GEN.numero
         
         order by CPT.numero
         
         /*A
         ** Liste des GTC FAV Tunnel    
         */
         
         select GEN.maitre,
         	FAV.numero, 
         	FAV.voie, 
         	FAV.rang_comm, 
         	FAV.rang_comm2, 
         	FAV.rang_etat, 
         	FAV.rang_etat2, 
         	FAV.bit_disc, 
         	FAV.bit_disc2 
         	
         from CFT..EQT_FAV FAV, CFT..EQT_GEN GEN
         
         where GEN.type	  = XDC_EQT_FAV
           and GEN.eqt_supprime = XDC_FAUX
           and GEN.numero = FAV.numero
         
         order by FAV.numero
         
         /*A
         ** Liste des GTC Panneau    
         */
         
         select GEN.maitre,
         	GEN.type_maitre,
         	PAN.numero, 
         	PAN.rang_comm, 
         	PAN.rang_comm2, 
         	PAN.rang_etat, 
         	PAN.rang_etat2, 
         	PAN.bit_disc, 
         	PAN.bit_disc2 
         	
         from CFT..EQT_PAN PAN, CFT..EQT_GEN GEN
         
         where GEN.type	  = XDC_EQT_PAN
           and GEN.eqt_supprime = XDC_FAUX
           and PAN.numero = GEN.numero
         
         order by PAN.numero
         
         end
         
      end
#endif
      
   end
      
   return XDC_OK
         
go  
