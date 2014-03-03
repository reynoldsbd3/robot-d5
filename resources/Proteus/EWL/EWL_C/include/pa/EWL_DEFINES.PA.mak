E200z0_V_S_CONFIG	= E200z0_VLE_Soft
E200z1_V_S_CONFIG	= E200z1_VLE_Soft
E200z3_V_CONFIG		= E200z3_VLE
E200z3_V_SP_CONFIG	= E200z3_VLE_SPFP_Only
E200z4_V_CONFIG		= E200z4_VLE
E200z4_V_SP_CONFIG	= E200z4_VLE_SPFP_Only
E200z6_CONFIG		= E200z6
E200z6_SP_CONFIG	= E200z6_SPFP_Only
E200z6_V_CONFIG		= E200z6_VLE
E200z6_V_SP_CONFIG	= E200z6_VLE_SPFP_Only
E200z7_V_CONFIG		= E200z7_VLE
E200z7_V_SP_CONFIG	= E200z7_VLE_SPFP_Only

BASIC_ALIGN_4_CFLAGS			= -char unsigned -big -opt peep -func_align 4 -use_lmw_stmw on -schedule on

BASIC_ALIGN_4_SOFT_CFLAGS		= -fp soft $(BASIC_ALIGN_4_CFLAGS)
BASIC_ALIGN_4_SPFP_SPE1_CFLAGS		= -fp spfp -spe_vector -use_isel on $(BASIC_ALIGN_4_CFLAGS)
BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS	= -fp spfp_only -spe_vector -use_isel on $(BASIC_ALIGN_4_CFLAGS)

$(E200z0_V_S_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SOFT_CFLAGS)
$(E200z1_V_S_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SOFT_CFLAGS)
$(E200z3_V_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z3_V_SP_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)
$(E200z4_V_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z4_V_SP_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)
$(E200z6_CONFIG)_CFLAGS		= -proc Zen -use_isel on $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z6_SP_CONFIG)_CFLAGS	= -proc Zen -use_isel on $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)
$(E200z6_V_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z6_V_SP_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)
$(E200z7_V_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SPFP_SPE1_CFLAGS)
$(E200z7_V_SP_CONFIG)_CFLAGS	= -proc Zen -vle -ppc_asm_to_vle -use_isel on $(BASIC_ALIGN_4_SPFP_ONLY_SPE1_CFLAGS)
