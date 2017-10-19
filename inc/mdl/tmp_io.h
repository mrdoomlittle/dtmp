# ifndef __tmp__io
# define __tmp__io
# include <mdlint.h>
# include <stdlib.h>
# include <math.h>
# define __DEBUG_ENABLED
# ifdef __AVR
#	include <avr/interrupt.h>
# endif

# ifdef __DEBUG_ENABLED
#	include <stdio.h>
# endif

//# define __TMP_LIGHT
# define __TMP_KOS

# define TMP_SUCCESS 0
# define TMP_TIMEO 1
# define TMP_FAILURE -1

# define ACK_SUCCESS 1
# define ACK_FAILURE 0

# define TMP_OPT_FLIP_BIT 0b10000000
# define TMP_OPT_INV_TX_TRIG_VAL 0b01000000
# define TMP_OPT_INV_RX_TRIG_VAL 0b00100000

# define TMP_DB_ONLY_ERR
// key and sync
# define TMP_FLG_KAS 0b10000000
# define TMP_FLG_NORET 0b01000000
# define TMP_DEF_FLAGS TMP_FLG_KAS
# define TMP_MAX_TRY_C 10
# define TMP_FLG_RBS 0b1000
# define TMP_FLG_CTMSG 0b0100
enum {
	_ctmsg_route
};

typedef mdl_u32_t tmp_addr_t;
typedef mdl_u8_t tmp_version_t;
typedef mdl_u8_t tmp_method_t;
typedef mdl_u8_t tmp_flag_t;
typedef mdl_i8_t tmp_err_t;

struct tmp_ctmsg {
	mdl_u8_t kind;
	mdl_u32_t lu_addr;
};

struct tmp_iface {
	tmp_addr_t addr;
	mdl_u8_t no;
};

struct tmp_io {
	mdl_u8_t rx_pid, tx_pid;
	mdl_u8_t rx_ci_pid, tx_ci_pid;
	mdl_u8_t rx_co_pid, tx_co_pid;

	mdl_uint_t snd_timeo, rcv_timeo;
	mdl_uint_t snd_timeo_ic, rcv_timeo_ic;

	mdl_uint_t snd_holdup_ic, rcv_holdup_ic;
	mdl_uint_t snd_holdup, rcv_holdup;
	tmp_flag_t snd_optflags, rcv_optflags;

	void(*set_pin_mode_fp)(mdl_u8_t, mdl_u8_t);
	void(*set_pin_state_fp)(mdl_u8_t, mdl_u8_t);
	mdl_u8_t(*get_pin_state_fp)(mdl_u8_t);
	void(*holdup_fp)(mdl_uint_t);
	tmp_flag_t flags;
# ifndef __TMP_LIGHT
	void(*set_iface_no_fp)(mdl_u8_t);
	mdl_u8_t(*get_iface_no_fp)();
	struct tmp_iface *iface;
	mdl_u8_t iface_c;
# endif
};

# ifndef __TMP_LIGHT
void __inline__ static tmp_set_iface_addr(struct tmp_io *__tmp_io, tmp_addr_t __addr, mdl_u8_t __no) {
	__tmp_io->iface[__no].addr = __addr;
}
void __inline__ static tmp_set_iface_no(struct tmp_io *__tmp_io, mdl_u8_t __no) {
	__tmp_io->set_iface_no_fp(__no);
}
mdl_u8_t __inline__ static tmp_get_iface_no(struct tmp_io *__tmp_io) {
	return __tmp_io->get_iface_no_fp();
}
__inline__ static struct tmp_iface *get_iface(struct tmp_io *__tmp_io, mdl_u8_t __no) {
	return __tmp_io->iface+__no;
}

struct tmp_iface* tmp_add_iface(struct tmp_io *__tmp_io, tmp_addr_t, mdl_u8_t);
void tmp_rm_iface(struct tmp_io *__tmp_io, struct tmp_iface*);
tmp_err_t tmp_snd_ack(struct tmp_io*, mdl_u8_t);
tmp_err_t tmp_rcv_ack(struct tmp_io*, mdl_u8_t*);
# endif

typedef mdl_i8_t tmp_err_t;

# ifndef __TMP_LIGHT
//tmp_err_t 
//tmp_err_t 

mdl_u32_t tmp_addr_from_str(char const*, tmp_err_t*);
# endif

typedef struct {
	mdl_u8_t *p;
	mdl_uint_t bc;
} tmp_io_buf_t;

# define TMP_PACKET_SIZE 128
# define TMP_PK_HEADER_SIZE 96
struct tmp_packet {
	tmp_addr_t dst_addr, src_addr;
	mdl_u32_t dt_sect_sv;
	tmp_io_buf_t io_buff;
};

tmp_err_t tmp_seti_speed(struct tmp_io*, mdl_uint_t);
tmp_err_t tmp_seto_speed(struct tmp_io*, mdl_uint_t);

# ifndef __TMP_LIGHT
tmp_err_t tmp_send_packet(struct tmp_io*, struct tmp_packet*);
tmp_err_t tmp_recv_packet(struct tmp_io*, struct tmp_packet*);
# endif

mdl_u8_t extern tmp_errno;
tmp_err_t tmp_init(struct tmp_io*, void(*)(mdl_u8_t, mdl_u8_t), void(*)(mdl_u8_t, mdl_u8_t), mdl_u8_t(*)(mdl_u8_t)
# ifndef __TMP_LIGHT
, tmp_method_t, tmp_flag_t
# endif
);

void tmp_set_holdup_fp(struct tmp_io*, void(*)(mdl_uint_t));
void tmp_holdup(struct tmp_io*, mdl_uint_t, mdl_uint_t);

tmp_io_buf_t tmp_io_buff(void*, mdl_uint_t);

void tmp_flip_rx_clk_trig_val();
mdl_u8_t tmp_is_rx_clk_trig_val(mdl_u8_t);

void tmp_flip_tx_clk_trig_val();
mdl_u8_t tmp_is_tx_clk_trig_val(mdl_u8_t);

tmp_err_t tmp_raw_snd(struct tmp_io*, tmp_io_buf_t);
tmp_err_t tmp_raw_rcv(struct tmp_io*, tmp_io_buf_t);

mdl_u8_t tmp_par_arcv_sig(struct tmp_io*);
mdl_u8_t tmp_par_asnd_sig(struct tmp_io*);
# ifndef __TMP_LIGHT
tmp_err_t tmp_send(struct tmp_io*, tmp_io_buf_t, tmp_addr_t);
tmp_err_t tmp_recv(struct tmp_io*, tmp_io_buf_t, tmp_addr_t);
# else
tmp_err_t tmp_send(struct tmp_io*, tmp_io_buf_t);
tmp_err_t tmp_recv(struct tmp_io*, tmp_io_buf_t);
# endif

tmp_err_t tmp_rcv_nib(struct tmp_io*, mdl_u8_t*);
tmp_err_t tmp_snd_nib(struct tmp_io*, mdl_u8_t);
tmp_err_t tmp_rcv_byte(struct tmp_io*, mdl_u8_t*);
tmp_err_t tmp_snd_byte(struct tmp_io*, mdl_u8_t);

tmp_err_t tmp_rcv_bit(struct tmp_io*, mdl_u8_t*);
tmp_err_t tmp_snd_bit(struct tmp_io*, mdl_u8_t);

# ifndef __TMP_LIGHT
tmp_err_t tmp_snd_64l(struct tmp_io*, mdl_u64_t);
tmp_err_t tmp_rcv_64l(struct tmp_io*, mdl_u64_t*);

tmp_err_t tmp_snd_32l(struct tmp_io*, mdl_u32_t);
tmp_err_t tmp_rcv_32l(struct tmp_io*, mdl_u32_t*);

tmp_err_t tmp_snd_16l(struct tmp_io*, mdl_u16_t);
tmp_err_t tmp_rcv_16l(struct tmp_io*, mdl_u16_t*);

tmp_err_t __inline__ static tmp_snd_8l(struct tmp_io *__tmp_io, mdl_u8_t __data) {
	return tmp_snd_byte(__tmp_io, __data);}
tmp_err_t __inline__ static tmp_rcv_8l(struct tmp_io *__tmp_io, mdl_u8_t *__data_p) {
	return tmp_rcv_byte(__tmp_io, __data_p);}
# endif

enum tmp_opt {TMP_OPT_SND_TIMEO, TMP_OPT_RCV_TIMEO};

typedef enum tmp_opt tmp_opt_t;
void tmp_set_opt(struct tmp_io*, tmp_opt_t, void*);
void tmp_get_opt(struct tmp_io*, tmp_opt_t, void*);

mdl_u8_t __inline__ static tmp_timeo(mdl_uint_t *__timeo_ic, mdl_uint_t __timeo) {
	if (!__timeo) return 0;
	if (*__timeo_ic >= __timeo) {
		*__timeo_ic = 0;
		return 1;
	}
	(*__timeo_ic)++;
	return 0;
}

mdl_u8_t __inline__ static tmp_snd_timeo(struct tmp_io *__tmp_io) {
	mdl_u8_t res = tmp_timeo(&__tmp_io->snd_timeo_ic, __tmp_io->snd_timeo);
	tmp_holdup(__tmp_io, 1, 1);
	return res;
}

mdl_u8_t __inline__ static tmp_rcv_timeo(struct tmp_io *__tmp_io) {
	mdl_u8_t res = tmp_timeo(&__tmp_io->rcv_timeo_ic, __tmp_io->rcv_timeo);
	tmp_holdup(__tmp_io, 1, 1);
	return res;
}

mdl_u8_t tmp_is_flag(mdl_u8_t, mdl_u8_t);
void tmp_tog_flag(mdl_u8_t*, mdl_u8_t);

void __inline__ static tmp_tog_snd_optflag(struct tmp_io *__tmp_io, mdl_u8_t __optflag) {
	tmp_tog_flag(&__tmp_io->snd_optflags, __optflag);}

mdl_u8_t __inline__ static tmp_is_snd_optflag(struct tmp_io *__tmp_io, mdl_u8_t __optflag) {
	return tmp_is_flag(__tmp_io->snd_optflags, __optflag);}

void __inline__ static tmp_tog_rcv_optflag(struct tmp_io *__tmp_io, mdl_u8_t __optflag) {
	tmp_tog_flag(&__tmp_io->rcv_optflags, __optflag);}

mdl_u8_t __inline__ static tmp_is_rcv_optflag(struct tmp_io *__tmp_io, mdl_u8_t __optflag) {
	return tmp_is_flag(__tmp_io->rcv_optflags, __optflag);}
# endif /*__tmp__io*/
