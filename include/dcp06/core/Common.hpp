// ====================================================================================================================
//
// Project  : Pluto/Venus Onboard Applications SW
//
// Component: 
//
// $Workfile: HEW_DCP05GUI.hpp $
//
// Summary  : 
//
// ------------------------------------------------------------------------------------------------
//
// Copyright by Leica Geosystems AG, Heerbrugg 2002
//
// ================================================================================================


// $Author: Hlar $
// $Date: 6.07.04 8:55 $
// $Revision: 1 $
// $Modtime: 5.07.04 14:55 $

/* $ History: $
*/
// $NoKeywords: $

#ifndef DCP_COMMON_HPP
#define DCP_COMMON_HPP

// ================================================================================================
// ==========================================  Includes   =========================================
// ================================================================================================

#include <dcp06/core/Types.hpp>
#include <dcp06/core/Model.hpp>
#include <dcp06/core/Defs.hpp>
#include <GUI_AppBase.hpp>



// Description: The Hello World application
//
namespace DCP
{
    // Forward declaration
	//class DCP06ModelC;
	//class DCP06ToolDlgC;


    // Description: Tabbed controller for the Hello World application
    class DCP06CommonC
    {
        public:

            // Description: Constructor
            DCP06CommonC(DCP06ModelC* pDCP06Model);
			// DCP06CommonC();
			~DCP06CommonC();
			  
			bool check_distance(double x, double y, double z, S_POINT_BUFF* points, short iCount, short iCurrentPoint);
			double to_mm(double dist);

			short defined_points_count_in_line(S_LINE_BUFF *line, short *lastpoint);
			short defined_points_count_in_plane(S_PLANE_BUFF *plane,short *lastpoint);

			short get_max_defined_point_line(S_LINE_BUFF *lines);
			short points_count_in_plane(S_PLANE_BUFF* plane);
			short get_last_defined_point(S_POINT_BUFF* points, short max);
			short  get_last_defined_point(S_POINT_BUFF *point_OCS, S_POINT_BUFF *point_DCS, short max);
			short get_OCS_points_count(S_POINT_BUFF *point_OCS, short max);
			void empty_xyz_buffers(char *x, char *y, char *z, short pit);
			void empty_buffer(char *xyz, short pit);
			short strblank(char *s);
			char *strlower(char *s);
			void delete_point(S_POINT_BUFF *points);
			char *strbtrim(char *s); 
			char *strtrim(char *s);
			char *strltrim(char *s);

			short defined_pom_points(S_POINT_BUFF *point_OCS, short *lastpoint);
			short get_OCS_SCS_points_count(S_POINT_BUFF *point_OCS,S_POINT_BUFF *point_DCS, short max);
			void copy_xyz_to_buffer(double *x, double *y, double *z, char *bX, char *bY, char *bZ, short pit, short dec);
			void copy_xyz_to_buffer_f(float *x, float *y, float *z, char *bX, char *bY, char *bZ, short pit, short dec);

			short overwrite_point(StringC sPid);
			void to_xyz(double dis, double hor, double ver, double *x, double *y, double *z, short tool, double *x_scs, double *y_scs, double *z_scs);
			void to_AMS_xyz(double x_in, double y_in, double z_in, double *x_out, double *y_out, double *z_out);

			unsigned int get_free_space();
			short check_free_space(unsigned int si);
			short card_status();
			short read_allow_edit();
			short GetDate(int* day, int* month, int* year);
			short GetTime(int* iHour, int* iMin, int* iSec);
			short GetUserName(char *name);
			short GetInstrumentNo(int* no);
			short convert_to_ascii(StringC from, char *to, short iLen);
			
			short GetInstrumentName(char* intrument_name);
			void get_dist_(char *act, char *des, char *dist);

			StringC get_info_text(short& iInd);

			short get_rotation_status();

			void get_dist_len1(char *act, char *des, char *dist, short len);
			void get_dist_len(char *act, char *des, char *dist, short len);
			void calc_di(char *tulos,char *x1, char *y1, char *z1,char *x2, char *y2, char *z2);

			short defined_circle_points(S_CIRCLE_BUFF *circles, short *lastpoint);
			short get_max_defined_point_circle(S_CIRCLE_BUFF *circles);

			void inc_id(char *id);


			bool check_edm_mode();
			void delay();

			short find_first_file(const char* file_name, boost::filesystem::path* FileInfo);

			short calc_mid_point(S_POINT_BUFF *p1, S_POINT_BUFF *p2, double *x, double *y, double *z);

			short calc_mid_point(S_POINT_BUFF *points ,double *x, double *y, double *z);

			DCP06ModelC *m_pDCP06Model;
	private:
			//DCP06ModelC *m_pDCP06Model;
    };
};

#endif // DCP_BASEGUI_HPP




