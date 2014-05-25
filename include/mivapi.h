/*
 * vi:set ts=4 sts=4 sw=4:
 * $Id: mivapi.h 35455 2013-03-11 21:56:41Z burch $
 */

/*
 * This file and the source codes contained herein are the property of
 * Miva Merchant, Inc.  Use of this file is restricted to the specific terms and
 * conditions in the License Agreement associated with this file.  Distribution
 * of this file or portions of this file for uses not covered by the License
 * Agreement is not allowed without a written agreement signed by an officer of
 * Miva Merchant, Inc.
 *
 * Copyright 1998-2013 Miva Merchant, Inc.  All rights reserved.
 * http://www.mivamerchant.com
 */

#ifndef __mivapi_h
#define __mivapi_h

#ifdef __cplusplus
extern "C"
{
#endif

typedef void *		mvProgram;
typedef void *		mvConfig;
typedef void *		mvVariable;
typedef void *		mvDatabase;
typedef void *		mvDatabaseView;
typedef void *		mvDatabaseVariable;
typedef void *		mvVariableHash;
typedef void *		mvVariableList;
typedef void *		mvNetworkConnection;
typedef void *		mvFile;
typedef void *		mvSystemLibrary;
typedef int			mvCommerce_Status;
typedef int			mvConfig_Status;
typedef void *		mvDynamicExpression;

typedef void ( *mvPersistent_Callback )( mvProgram, void * );
typedef int  ( *mvProgram_Output_Callback )( const char *, int );

#define MV_EL_FUNCTION_VERSION		1
#define MV_EL_COMMERCE_VERSION		1
#define MV_EL_DATABASE_VERSION		6
#define MV_EL_CONFIG_VERSION		2

#define MIVA_LENGTH_ASCIZ			-1

/*
 * MV_EL_Libvmconfig
 */

typedef struct _MV_EL_VMConfig
{
	int		id;
	int		int_value;
	char	*value;
} MV_EL_VMConfig;

/*
 * mvProgram functions
 */

mvVariable			mvProgram_Lookup_Variable( mvProgram program, const char *name, int name_length );

void				mvProgram_Local_Variables( mvProgram program, mvVariableList list );
void				mvProgram_System_Variables( mvProgram program, mvVariableList list );
void				mvProgram_Global_Variables( mvProgram program, mvVariableList list );

int					mvProgram_Compare( mvProgram program, mvVariable left, mvVariable right );

void				mvProgram_SetLanguage( mvProgram program, const char *language, int language_len );
void				mvProgram_SetDefaultLanguage( mvProgram program, const char *language, int language_len );
void				mvProgram_SetDefaultDatabase( mvProgram program, const char *database, int database_len );

void *				mvProgram_Allocate( mvProgram program, int size );
void				mvProgram_Free( mvProgram program, void *data );

int					mvProgram_Output_Header( mvProgram program, const char *name, int name_length, int name_del, const char *value, int value_length, int value_del );
int					mvProgram_Output( mvProgram program, const char *data, int length );
void				mvProgram_Output_Flush( mvProgram program );

char *				mvProgram_Attribute_Encode( mvProgram program, const char *data, int data_length, int *result_length, int *result_size );
char *				mvProgram_Attribute_Decode( mvProgram program, const char *data, int data_length, int *result_length, int *result_size );

char *				mvProgram_Entity_Encode( mvProgram program, const char *data, int data_length, int *result_length, int *result_size );
char *				mvProgram_Entity_Decode( mvProgram program, const char *data, int data_length, int *result_length, int *result_size );

void *				mvProgram_Lookup_Persistent( mvProgram program, const char *key, int key_length );
void				mvProgram_Register_Persistent( mvProgram program, const char *key, int key_length, void *data, mvPersistent_Callback cleanup );

void *				mvProgram_SSL_Function( mvProgram program, const char *function_name );
mvSystemLibrary		mvProgram_SystemLibrary( mvProgram program, const char *code );

char *				mvProgram_MakeSessionID( mvProgram program, int *length );

int					mvProgram_RunFunction( mvProgram program, const char *function, int function_len, mvVariableList params, mvVariable returnvalue );

mvDynamicExpression	mvProgram_Compile_Dynamic_Expression( mvProgram program, const char *expr, int expr_length, char **error, int *error_length );
int					mvProgram_Execute_Dynamic_Expression( mvProgram program, mvDynamicExpression expr, mvVariableHash localvar_hash, mvVariable result );
int					mvProgram_Execute_Dynamic_Expression_Boolean( mvProgram program, mvDynamicExpression expr, mvVariableHash localvar_hash, int *result );

mvVariableHash		mvProgram_Global_VariableHash( mvProgram program );
mvVariableHash		mvProgram_System_VariableHash( mvProgram program );

void				mvProgram_ResetGlobalTimeout( mvProgram program );
void				mvProgram_Sleep( mvProgram program, int msecs );

int					mvProgram_FatalError( mvProgram program, const char *error, int error_length );

/*
 * mvSystemLibrary definitions
 */

void *				mvSystemLibrary_GetFunction( mvSystemLibrary library, const char *function_name );
const char *		mvSystemLibrary_Error( mvProgram program, mvSystemLibrary library, int *error_length );

/*
 * mvCommerce definitions
 */

#define MVC_OK			0
#define MVC_ERROR		1
#define MVC_END			2

/*
 * Variable functions
 */

mvVariable			mvVariable_Allocate( const char *name, int name_length, const char *value, int value_length );
mvVariable			mvVariable_Allocate_Copy( mvVariable variable );

mvVariable			mvVariable_NextIdentity( mvVariable variable );
mvVariable			mvVariable_RealIdentity( mvVariable variable );
void				mvVariable_Make_Reference( mvVariable variable, mvVariable value );

const char *		mvVariable_Name( mvVariable variable, int *length );

int					mvVariable_Value_Integer( mvVariable variable );
double				mvVariable_Value_Double( mvVariable variable );
const char *		mvVariable_Value( mvVariable variable, int *length );

int					mvVariable_SetValue_Integer( mvVariable variable, int value );
int					mvVariable_SetValue_Double( mvVariable variable, double value );
int					mvVariable_SetValue( mvVariable variable, const char *value, int length );
int					mvVariable_SetValue_Variable( mvVariable variable, mvVariable value );
int					mvVariable_SetValue_Nocopy( mvVariable variable, char *value, int length, int size );

void				mvVariable_Free( mvVariable variable );

/*
 * Aggregate identification functions
 */

#define MVA_VALUE	0
#define MVA_ARRAY	1
#define MVA_STRUCT	2

int					mvVariable_Aggregate_Type( mvVariable var );

/*
 * Aggregate iteration
 */

void				mvVariable_Aggregate_List( mvVariable agg, mvVariableList list );

/*
 * Aggregate data functions
 */

void				mvVariable_Set_Array_Element( int index, mvVariable value, mvVariable agg );
void				mvVariable_Insert_Array_Element( int index, mvVariable value, mvVariable agg );
void				mvVariable_Delete_Array_Elements( int start, int count, int collapse, mvVariable agg );
void				mvVariable_Array_Merge( mvVariable source, int spos, int scount, mvVariable dest, int dpos, int copy );
void				mvVariable_Array_Copy( mvVariable source, int spos, int scount, mvVariable dest, int dpos, int copy );

void				mvVariable_Set_Struct_Member( const char *member_name, int member_name_length, mvVariable value, mvVariable agg );
void				mvVariable_Delete_Struct_Member( const char *member_name, int member_name_length, mvVariable agg );

mvVariable			mvVariable_Array_Element( int index, mvVariable agg, int create );
mvVariable			mvVariable_Struct_Member( const char *member_name, int member_name_length, mvVariable agg, int create );

/*
 * Aggregate metadata functions
 */

int					mvVariable_Array_Max( mvVariable agg );
int					mvVariable_Array_Min( mvVariable agg );
int					mvVariable_Array_Next( mvVariable agg, int index );
int					mvVariable_Array_Previous( mvVariable agg, int index );
int					mvVariable_ElementsUsed( mvVariable agg );

/*
 * VariableList functions
 */

mvVariableList		mvVariableList_Allocate();

mvVariable			mvVariableList_First( mvVariableList list );
mvVariable			mvVariableList_Current( mvVariableList list );
mvVariable			mvVariableList_Next( mvVariableList list );
int					mvVariableList_Entries( mvVariableList list );

mvVariable			mvVariableList_Find( mvVariableList list, const char *name, int name_length );
void				mvVariableList_Dump( mvVariableList list );
void				mvVariableList_SetVariable( mvVariableList list, const char *name, int name_length, const char *value, int value_length );
void				mvVariableList_Insert( mvVariableList list, mvVariable var );

void				mvVariableList_Free( mvVariableList list );

/*
 * VariableHash functions
 */

mvVariableHash		mvVariableHash_Allocate();

mvVariable			mvVariableHash_Index( mvVariableHash hash, int index );
mvVariable			mvVariableHash_Find( mvVariableHash hash, const char *name, int name_length );
void				mvVariableHash_Insert( mvVariableHash hash, mvVariable variable );

void				mvVariableHash_Dump( mvVariableHash hash );
void				mvVariableHash_SetVariable( mvVariableHash hash, const char *name, int name_length, const char *value, int value_length );
void				mvVariableHash_ReplaceVariable( mvVariableHash hash, mvVariable variable );

void				mvVariableHash_Free( mvVariableHash hash );

/*
 * MivAPI Networking Functions
 */

int					mvNetwork_Is_IP_Address( mvProgram program, const char *ipaddress );
int					mvNetwork_Get_Host_Address( mvProgram program, const char *hostname, char *ipaddress, int length );
mvNetworkConnection	mvNetwork_Connect( mvProgram program, const char *hostname, int port );
mvNetworkConnection	mvNetwork_Connect_SSL( mvProgram program, const char *hostname, int port );
mvNetworkConnection	mvNetwork_Open_URL( mvProgram program, const char *method, const char *url, const char *content, const char *contenttype, int contentlength );

int					mvNetwork_Read( mvNetworkConnection connection, char *buffer, int size, int timeout );
int					mvNetwork_Read_Line( mvNetworkConnection connection, char *line, int length, int timeout );
int					mvNetwork_Write( mvNetworkConnection connection, char *buffer, int size );
void				mvNetwork_Close( mvNetworkConnection connection );
const char *		mvNetwork_Error( mvProgram program, mvNetworkConnection connection );

/*
 * MivAPI File Functions
 */

#define MVF_SCRIPT            0x01
#define MVF_DATA              0x02
#define MVF_COMMERCE          0x03

#define MVF_MODE_READ          0x01
#define MVF_MODE_WRITE         0x02
#define MVF_MODE_CREATE        0x04
#define MVF_MODE_TRUNCATE      0x08
#define MVF_MODE_APPEND        0x10
#define MVF_MODE_EXCLUSIVE     0x20

#define MVF_LOCK_READ          0x01
#define MVF_LOCK_WRITE         0x02

int					mvFile_Resolve( mvProgram program, int location, const char *path, int path_length, char **resolved, int *resolved_length );
mvFile				mvFile_Open( mvProgram program, int location, const char *path, int path_length, int mode );
int					mvFile_Delete( mvProgram program, int location, const char *path, int path_length );
int					mvFile_Rename( mvProgram program, int oldlocation, const char *oldpath, int oldpath_length, int newlocation, const char *newpath, int newpath_length );
int					mvFile_Exists( mvProgram program, int location, const char *path, int path_length );
long				mvFile_Size( mvProgram program, int location, const char *path, int path_length );
int					mvFile_Time( mvProgram program, int location, const char *path, int path_length );

int					mvFile_IsDirectory( mvProgram program, int location, const char *path, int path_length );
int					mvFile_MakeDirectory( mvProgram program, int location, const char *path, int path_length );
int					mvFile_RemoveDirectory( mvProgram program, int location, const char *path, int path_length );

int					mvFile_Mode( mvProgram program, int location, const char *path, int path_length );
int					mvFile_Change_Mode( mvProgram program, int location, const char *path, int path_length, int mode );
char				mvFile_Path_Separator( mvProgram program );
int					mvFile_Copy( mvProgram program, int sourcelocation, const char *sourcepath, int sourcepath_length, int destlocation, const char *destpath, int destpath_length );
int					mvFile_MakeSymbolicLink( mvProgram program, int sourcelocation, const char *sourcepath, int sourcepath_length, int destlocation, const char *destpath, int destpath_length );

int					mvFile_OwnerID( mvFile file );
int					mvFile_GroupID( mvFile file );

int					mvFile_Read( mvFile file, char *buffer, int size );
int					mvFile_Write( mvFile file, const char *buffer, int size );

long				mvFile_Seek( mvFile file, long position );
long				mvFile_Tell( mvFile file );
long				mvFile_Length( mvFile file );
int					mvFile_Lock( mvFile file, int type, long offset, long size );
int					mvFile_Release( mvFile file, long offset, long size );
void				mvFile_Close( mvFile file );

int					mvFile_LockFile( mvProgram program, int location, const char *lockfile_name, int lockfile_name_length );
mvFile				mvFile_MakeTemporary( mvProgram program, int location, const char *path, int path_length, int mode, char **filename, int *filename_length );

int					mvDirectory_Listing( mvProgram program, int location, const char *path, int path_length, mvVariableList output );

const char *		mvFile_Error( mvProgram program, mvFile file, int *length );

/*
 * Miva API Configuration definitions/functions
 */

/* Next value: 0x...29 */
#define MvCONFIG_OK                         0
#define MvCONFIG_ERROR                      1

#define MvCONFIG_API_CGI                    0
#define MvCONFIG_API_ISAPI                  1
#define MvCONFIG_API_MIA                    2

/* Values for MvCONFIG_FLAGS_SECURITY */
#define MvCONFIG_SEC_S_ALLOW_SYMLINKS       0x00000001
#define MvCONFIG_SEC_S_IGNORE_SYM_OWNERSHIP 0x00000002
#define MvCONFIG_SEC_D_ALLOW_SYMLINKS       0x00000004
#define MvCONFIG_SEC_D_IGNORE_SYM_OWNERSHIP 0x00000008
#define MvCONFIG_SEC_D_ALLOW_ABSOLUTE_PATHS 0x00000010
#define MvCONFIG_SEC_S_ALLOW_ABSOLUTE_PATHS 0x00000020
#define MvCONFIG_SEC_T_ALLOW_SYMLINKS       0x00000100
#define MvCONFIG_SEC_T_IGNORE_SYM_OWNERSHIP 0x00000200
#define MvCONFIG_SEC_SSL_NOVERIFY_CHAIN		0x00001000
#define MvCONFIG_SEC_SSL_NOVERIFY_HOSTNAME	0x00002000
#define MvCONFIG_SEC_SSL_VERIFY_IP			0x00004000

#define MvCONFIG_TYPE_INTEGER               0x01000000
#define MvCONFIG_TYPE_STRING                0x02000000
#define MvCONFIG_TYPE_NAMEVALUEPAIR         0x04000000      /* Currently only used by callable VM configuration. */
#define MvCONFIG_TYPE_MASK                  0x0F000000

#define MvCONFIG_DIR_MIVA                   0x02000001
#define MvCONFIG_DIR_COMMERCE               0x02000002
#define MvCONFIG_DIR_TEMP                   0x02000003
#define MvCONFIG_DIR_DATA                   0x02000004
#define MvCONFIG_DIR_CA                     0x02000016
#define MvCONFIG_DIR_USER                   0x02000019
#define MvCONFIG_DIR_BUILTIN                0x0200001e

#define MvCONFIG_LOG_LEVEL                  0x01000005
#define MvCONFIG_LOG_FILE                   0x02000006
#define MvCONFIG_LOG_FILTER					0x02000025
#define MvCONFIG_LOG_COOKIE					0x02000028

/* Values for MvCONFIG_LOG_LEVEL */
#define MvCONFIG_LOG_FUNC_CALL				0x00000010
#define MvCONFIG_LOG_FUNC_PARAM				0x00000020
#define MvCONFIG_LOG_FUNC_RETVAL			0x00000040
#define MvCONFIG_LOG_FUNC_RETURN			0x00000080
#define MvCONFIG_LOG_EXTERNAL_FUNC_CALL		0x00000100
#define MvCONFIG_LOG_EXTERNAL_FUNC_PARAM	0x00000200
#define MvCONFIG_LOG_EXTERNAL_FUNC_RETVAL	0x00000400
#define MvCONFIG_LOG_EXTERNAL_FUNC_RETURN	0x00000800
#define MvCONFIG_LOG_MvDO_FILE_ENTER		0x00001000
#define MvCONFIG_LOG_MvDO_FILE_EXIT			0x00002000
#define MvCONFIG_LOG_MvCALL					0x00100000
#define MvCONFIG_LOG_MvSMTP					0x00200000
#define MvCONFIG_LOG_APPLICATION_TIMEOUT	0x08000000
#define MvCONFIG_LOG_RUNTIME_ERROR			0x10000000
#define MvCONFIG_LOG_FATAL_ERROR			0x20000000
#define MvCONFIG_LOG_TRACE					0x40000000

#define MvCONFIG_TIMEOUT_GLOBAL             0x01000007
#define MvCONFIG_TIMEOUT_MAIL               0x01000008
#define MvCONFIG_TIMEOUT_CALL               0x01000009
#define MvCONFIG_TIMEOUT_FILE               0x0100000a
#define MvCONFIG_TIMEOUT_POST				0x01000026

#define MvCONFIG_DELAY_FILE                 0x0100000b

#define MvCONFIG_COOKIES                    0x0100000c

#define MvCONFIG_BUFSZ_IMPORT               0x0100000d
#define MvCONFIG_BUFSZ_TEMPLATE             0x0100000e
#define MvCONFIG_BUFSZ_CALL                 0x0100000f
#define MvCONFIG_BUFSZ_SMTP                 0x01000010
#define MvCONFIG_BUFSZ_CONTENT              0x01000011
#define MvCONFIG_BUFSZ_OUTPUT               0x01000012
#define MvCONFIG_BUFSZ_OUTPUT_FLUSH         0x0100001d

#define MvCONFIG_MAX_FILES                  0x01000013
#define MvCONFIG_MAX_DOCACHE                0x01000021
#define MvCONFIG_MAX_POST_VARS				0x01000027
#define MvCONFIG_MAX_UNKNOWN_CONTENT_LENGTH	0x01000028

#define MvCONFIG_LOCK_EXPIRATION            0x01000014
#define MvCONFIG_LOCK_HOSTNAME              0x02000015

#define MvCONFIG_DEFAULT_FILE               0x02000017

#define MvCONFIG_FLAGS_SECURITY             0x01000018

#define MvCONFIG_INFO_SERVERADMIN           0x0200001a

#define MvCONFIG_SSL_OPENSSL                0x0200001b
#define MvCONFIG_SSL_CRYPTO                 0x0200001c

/* The following symbols are used by the callable VM */
#define MvCONFIG_LIBRARY_COMMERCE			0x0400001f
#define MvCONFIG_LIBRARY_DATABASE			0x04000020
/* #define MvCONFIG_LIBRARY_BUILTIN is not needed, as DIR_BUILTIN scoops up all .so files in that directory as builtins. */

/* The following symbols are used by mvConfig_Set_DefaultErrorSetting */
#define MvCONFIG_ERRORSETTING_DISPLAY		0x00000001
#define MvCONFIG_ERRORSETTING_FATAL			0x00000002

mvConfig_Status		mvConfig_SetValue( mvConfig config, int id, const char *value, int value_length, int value_del );
mvConfig_Status		mvConfig_SetValue_Integer( mvConfig config, int id, int value );

mvConfig_Status		mvConfig_Register_Library_Commerce( mvConfig config, const char *method, const char *dll );
mvConfig_Status		mvConfig_Register_Library_Database( mvConfig config, const char *type, const char *dll );
mvConfig_Status		mvConfig_Register_Library_Builtin( mvConfig config, const char *dll );
mvConfig_Status		mvConfig_Register_Library_System( mvConfig config, const char *code, const char *dll );

void				mvConfig_SetVariable_Global( mvConfig config, const char *name, int name_length, const char *value, int value_length );
void				mvConfig_SetVariable_System( mvConfig config, const char *name, int name_length, const char *value, int value_length );
void				mvConfig_Set_DefaultErrorSetting( mvConfig config, const char *tagname, int tagname_length, int setting_value );

/*
 * MivAPI Database Functions
 */

void				mvDatabase_SetData( mvDatabase db, void *data );
void *				mvDatabase_data( mvDatabase db );
mvDatabaseView		mvDatabase_AddView( mvDatabase db, const char *name, int name_length, void *data );
void				mvDatabase_SetPrimaryView( mvDatabase db, mvDatabaseView dbview );
mvProgram			mvDatabase_Program( mvDatabase db );

mvDatabase			mvDatabaseView_Database( mvDatabaseView dbview );
void				mvDatabaseView_SetData( mvDatabaseView dbview, void *data );
void *				mvDatabaseView_data( mvDatabaseView dbview );

void				mvDatabaseView_Clear( mvDatabaseView dbview );
mvDatabaseVariable	mvDatabaseView_AddVariable( mvDatabaseView dbview, const char *name, int name_length, void *data );
void				mvDatabaseView_Commit( mvDatabaseView dbview );
void				mvDatabaseView_SetDirty( mvDatabaseView dbview );
mvProgram			mvDatabaseView_Program( mvDatabaseView dbview );

mvDatabaseView		mvDatabaseVariable_DatabaseView( mvDatabaseVariable dbvar );
void				mvDatabaseVariable_SetData( mvDatabaseVariable dbvar, void *data );
void *				mvDatabaseVariable_data( mvDatabaseVariable dbvar );
void				mvDatabaseVariable_SetDirty( mvDatabaseVariable dbvar );

/*
 * MV_EL_FunctionParameter
 */

typedef struct _MV_EL_FunctionParameter
{
	const char	*name;
	int			name_length;
	int			flags;
} MV_EL_FunctionParameter;

#define EPF_NORMAL		0x00000000
#define EPF_REFERENCE	0x00000001

/*
 * MV_EL_Function
 */

typedef struct _MV_EL_Function
{
	const char				*name;
	int						name_length;

	int						parameter_count;
	MV_EL_FunctionParameter	*parameters;

	void		( *function )( mvProgram, mvVariableHash, mvVariable, void ** );
} MV_EL_Function;

/*
 * MV_EL_Function_List
 */

typedef struct _MV_EL_Function_List
{
	int				version;
	MV_EL_Function	*list;
} MV_EL_Function_List;

/*
 * MivAPI functions for interrogating available external functions
 */

const MV_EL_Function *mvProgram_Lookup_ExternalFunction( mvProgram program, const char *function, int function_len );

/*
 * MV_EL_Commerce
 */

typedef struct _MV_EL_Commerce
{
	int version;
	mvCommerce_Status		( *initfunction )( mvProgram prog,
											   void **data,
											   const char *method,
											   const char *action,
											   mvVariableList input,
											   mvVariableHash output );
	mvCommerce_Status		( *loopfunction )( mvProgram prog,
											   void **data,
											   const char *method,
											   const char *action,
											   mvVariableList input,
											   mvVariableHash output,
											   int iteration );
	void		( *cleanupfunction )( mvProgram prog,
									  void **data,
									  const char *method,
								  	  const char *action,
									  mvVariableList input );
	const char *( *errorfunction )(	mvProgram prog,
									void **data,
									const char *method,
									const char *action,
									mvVariableList input );
} MV_EL_Commerce;

/*
 * MV_EL_Config
 */

typedef struct _MV_EL_Config
{
	int					version;

	mvConfig_Status		( *init )( mvConfig config, void **data, int apitype, void *apidata, int apidata_size );

	/*
	 * NOTE: As of Config interface v2, this function is deprecated.  It will never be called for v2 libraries, and v1 libraries
	 *       that provide it will no longer be allowed.
	 */

	mvConfig_Status		( *load_script_config )( mvConfig config, void **data, const char *filepath, const char *errorpath );

	mvConfig_Status		( *value_integer )( mvConfig config, void **data, int id, int *value );
	mvConfig_Status		( *value_string )( mvConfig config, void **data, int id, const char **value, int *value_length, int *value_del );

	const char *		( *error )( mvConfig config, void **data );

	void				( *cleanup )( mvConfig config, void **data );

	/* Following are Config interface version 2 functions */
	mvConfig_Status		( *load_script_config_v2 )( mvConfig config, void **data, const char *filepath, mvFile file );
} MV_EL_Config;

/*
 * MV_EL_Database
 */

#define MVD_TOP            -1
#define MVD_BOTTOM         -2

#define MVD_TYPE_NONE		0
#define MVD_TYPE_STRING		1
#define MVD_TYPE_INTEGER	2
#define MVD_TYPE_DOUBLE		3

typedef struct _MV_EL_Database
{
	int version;

	int			( *db_create )				( mvDatabase db,
											  const char *path,		int path_length,
											  const char *name,		int name_length,
											  const char *fields,	int fields_length );
	int			( *db_open )				( mvDatabase db,
											  const char *path,		int path_length,
											  const char *name,		int name_length,
											  const char *user,		int user_length,
											  const char *password,	int password_length,
											  const char *flags,	int flags_length );
	int			( *db_close )				( mvDatabase db );
	int			( *db_openview )			( mvDatabase db,
											  const char *name,		int name_length,
											  const char *query,	int query_length,
											  mvVariableList list, int entries );
	int			( *db_runquery )			( mvDatabase db,
											  const char *query,	int query_length,
											  mvVariableList list, int entries );
	int			( *db_pack )				( mvDatabase db );
	int			( *db_setprimaryindex )		( mvDatabase db,
											  const char *indexname,int indexname_length );
	int			( *db_createindex )			( mvDatabase db,
											  const char *indexfile,int indexfile_length,
											  const char *expression,int expression_length,
											  const char *flags,	int flags_length );
	int			( *db_openindexes )			( mvDatabase db,
											  const char *indexname,int indexname_length );
	int			( *db_reindex )				( mvDatabase db );

	const char*	( *db_error )				( mvDatabase db );

	int			( *dbview_close )			( mvDatabaseView dbview );
	int			( *dbview_skip )			( mvDatabaseView dbview,
											  int rows );
	int			( *dbview_go )				( mvDatabaseView dbview,
											  int row );
	int			( *dbview_add )				( mvDatabaseView dbview );
	int			( *dbview_delete )			( mvDatabaseView dbview );
	int			( *dbview_undelete )		( mvDatabaseView dbview );
	int			( *dbview_update )			( mvDatabaseView dbview );
	int			( *dbview_filter )			( mvDatabaseView dbview,
											  const char *filter,	int filter_length );
	int			( *dbview_find )			( mvDatabaseView dbview,
											  const char *search,	int search_length,
											  int exact );
	int			( *dbview_revealstructuretable )	( mvDatabaseView dbview,
											  const char *path,		int path_length );
	int			( *dbview_revealstructureagg )	( mvDatabaseView dbview,
											  mvVariable **array );

	const char* ( *dbview_error )			( mvDatabaseView dbview );

	int			( *dbvar_getvalue_int )		( mvDatabaseVariable dbvar,
											  int *value );
	int			( *dbvar_getvalue_double )	( mvDatabaseVariable dbvar,
											  double *value );
	int			( *dbvar_getvalue_string )	( mvDatabaseVariable dbvar,
											  char **value, int *value_length, int *value_del );

	int			( *dbvar_setvalue_int )		( mvDatabaseVariable dbvar,
											  int value );
	int			( *dbvar_setvalue_double )	( mvDatabaseVariable dbvar,
											  double value );
	int			( *dbvar_setvalue_string )	( mvDatabaseVariable dbvar,
											  const char *value, int value_length );

	void		( *dbvar_cleanup )			( mvDatabaseVariable dbvar );

	/* Following are DB interface version 2 functions: */
	int			( *db_commit )				( mvDatabase db );
	int			( *db_rollback )			( mvDatabase db );

	/* Following are DB interface version 4 functions: */
	int			( *dbvar_preferred_type )	( mvDatabaseVariable db );

	/* Following are DB interface version 5 functions: */
	int			( *db_transact )			( mvDatabase db );
	int			( *db_command )				( mvDatabase db, const char *command, int command_length, const char *parameter, int parameter_length );

	/* Following are DB interface version 6 functions: */
	void		( *db_cleanup )				( mvDatabase db );
} MV_EL_Database;

#ifdef __cplusplus
}
#endif

#endif
