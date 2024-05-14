/*E*/
/*  Fichier : $Id: xzao4001sp.prc,v 1.3 2022/01/11 12:44:04 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2022/01/11 12:44:04 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao4001sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module Objets Statiques
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	25/09/17 : Creation d'apres XZAO400 : colonne 'code_zone' de type smallint (DEM 1236)  1.1
* JPL	15/10/18 : Filtrage des equipements marques comme supprimes (DEM 1305)  1.2
* PNI	10/01/22 : Suppression des ZDP hors Escota 1.3
* PNI 30/06/22 : Suppression du filtre ZDP hors Escota SAE-495
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Liste des zones Temps De Parcours.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*
* SP	XZAO4001_Liste_ZDP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			: Base ou rechercher les donnees
* XDY_District	va_Site_in			: Site des equipements recherches
*
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* VALEUR RENDUE :
*
* Retourne la liste des Zone De Parcours.
*
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : equipement inconnu
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Retourne les parametres numero de la Zone De Parcours, numero , libelle, site etc ...
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO4001' and type = 'P')
	drop procedure XZAO4001
go


create procedure XZAO4001
	@va_Basedd_in		char(3),
	@va_Site_in		T_SITE	= null
as

   /*A
   ** Test de validite des parametres d'entree :
   */

   if @va_Basedd_in = null
      return XDC_ARG_INV

   if @va_Site_in = 0
      select @va_Site_in = null


      /*A
      **  Travail dans la base CFG
      */

      if @va_Basedd_in = XDC_BASE_CFG
      begin

         if ( @va_Site_in = NULL )
         begin

         /*A
         ** Recherche de la liste des Zone De Parcours pour tous les site
         */

              select distinct ZDP.numero,
           		      ZDP.sit,
        	   	      ZDP.libelle,
        		      GEN.autoroute,
        	   	      AUT.nom,
        	   	      GEN.maitre,
        	   	      GEN.PR,
           		      ZDP.PR_fin,
        	   	      GEN.sens,
           		      ZDP.TDP_habituel,
           		      ZDP.TDP_max,
           		      ZDP.tolerance_basse,
           		      ZDP.tolerance_haute,
           		      ZDP.type_zone,
           		      ZDP.code_zone,
			      rtrim(GEN.nom)

              from    CFG..EQT_GEN GEN,
        	      CFG..RES_AUT AUT,
        	      CFG..EQT_ZDP ZDP

                where GEN.type         = XDC_EQT_ZDP
#ifndef HIST
                  and GEN.eqt_supprime = XDC_FAUX
#endif
                  and GEN.numero       = ZDP.numero
                  and AUT.numero       = GEN.autoroute
                  and ZDP.inhibition   = 0
                  and ZDP.code_zone    != 0

                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero
         end

         else

         /*A
         ** Recherche de la liste des Zone De Parcours pour un site
         */

         begin

              select distinct ZDP.numero,
           		      ZDP.sit,
        	   	      ZDP.libelle,
        		      GEN.autoroute,
        	   	      AUT.nom,
        	   	      GEN.maitre,
        	   	      GEN.PR,
           		      ZDP.PR_fin,
        	   	      GEN.sens,
           		      ZDP.TDP_habituel,
           		      ZDP.TDP_max,
           		      ZDP.tolerance_basse,
           		      ZDP.tolerance_haute,
           		      ZDP.type_zone,
           		      ZDP.code_zone,
			      rtrim(GEN.nom)

              from    CFG..EQT_GEN GEN,
        	      CFG..RES_AUT AUT,
        	      CFG..EQT_ZDP ZDP

                where GEN.type         = XDC_EQT_ZDP
#ifndef HIST
                  and GEN.eqt_supprime = XDC_FAUX
#endif
                  and GEN.numero       = ZDP.numero
                  and AUT.numero       = GEN.autoroute
                  and ZDP.sit          = @va_Site_in
                  and ZDP.inhibition   = 0
                  and ZDP.code_zone    != 0

                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero
         end


      end

#ifdef CI

      else

      /*A
      **  Travail dans la base CFT
      */

      begin

         if ( @va_Site_in = NULL )
         begin

            /*A
            ** Recherche de la liste des Zone De Parcours pour tous les site
            */

              select distinct ZDP.numero,
           		      ZDP.sit,
        	   	      ZDP.libelle,
        		      GEN.autoroute,
        	   	      AUT.nom,
        	   	      GEN.maitre,
        	   	      GEN.PR,
           		      ZDP.PR_fin,
        	   	      GEN.sens,
           		      ZDP.TDP_habituel,
           		      ZDP.TDP_max,
           		      ZDP.tolerance_basse,
           		      ZDP.tolerance_haute,
           		      ZDP.type_zone,
           		      ZDP.code_zone,
			      rtrim(GEN.nom)

              from    CFT..EQT_GEN GEN,
        	      CFT..RES_AUT AUT,
        	      CFT..EQT_ZDP ZDP

                where GEN.type         = XDC_EQT_ZDP
#ifndef HIST
                  and GEN.eqt_supprime = XDC_FAUX
#endif
                  and GEN.numero       = ZDP.numero
                  and AUT.numero       = GEN.autoroute
                  and ZDP.inhibition   = 0
                  and ZDP.code_zone    != 0

                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero
         end

         else

         /*A
         ** Recherche de la liste des Zone De Parcours pour un site
         */

         begin

              select distinct ZDP.numero,
           		      ZDP.sit,
        	   	      ZDP.libelle,
        		      GEN.autoroute,
        	   	      AUT.nom,
        	   	      GEN.maitre,
        	   	      GEN.PR,
           		      ZDP.PR_fin,
        	   	      GEN.sens,
           		      ZDP.TDP_habituel,
           		      ZDP.TDP_max,
           		      ZDP.tolerance_basse,
           		      ZDP.tolerance_haute,
           		      ZDP.type_zone,
           		      ZDP.code_zone,
			      rtrim(GEN.nom)

              from    CFT..EQT_GEN GEN,
        	      CFT..RES_AUT AUT,
        	      CFT..EQT_ZDP ZDP

                where GEN.type         = XDC_EQT_ZDP
#ifndef HIST
                  and GEN.eqt_supprime = XDC_FAUX
#endif
                  and GEN.numero       = ZDP.numero
                  and AUT.numero       = GEN.autoroute
                  and ZDP.sit          = @va_Site_in
                  and ZDP.inhibition   = 0
                  and ZDP.code_zone    != 0

                order by GEN.sens, GEN.autoroute, GEN.PR, ZDP.numero
         end



      end

#endif

   return XDC_OK

go
