/*******************************************************************************
* Copyright (c) 2013, SHENZHEN SEMITEK Co.,LTD.
* All rights reserved.
*
* 文件名称: led.c
*
* 功能说明:
* 修改说明:
*******************************************************************************/
#include <string.h>
#include "om_driver.h"

#include "main.h"
#include "led.h"
#include "led_logo_tbl.h"

//-----------------------------------------------------------------------------;
// 常量定义区
//

//-----------------------------------------------------------------------------;
// 变量定义区
//

#define iLFSR_MULTINOMIAL       0xF521

bool                            bled_run_dir;

uint16_t                        lfsr_value;
uint8_t                         start_dec_count;
uint8_t                         snake_led_index;
extern uint8_t                  breach_color_index;
extern uint8_t                  on_color_index;

//-----------------------------------------------------------------------------;
// 表格定义区
//

//=============================================================================;
// 代码区                                                                      ;
//=============================================================================;

// 渐变类灯效统一: 输出0~255, 每帧都推进; 原来隔(档+1)帧才走一步、只有65级, 渐变一顿一顿
#define iFINE_SEG               1024            // 一段换色的相位长度

static uint8_t speed_lev(void)
{
    return (led_speed_lev > iSPEED_LEV_MAX) ? iSPEED_LEV_MAX : led_speed_lev;
}

static uint16_t fine_step(void)                 // 每帧相位增量 = 16/(档+1): 0档最快~4档最慢, 与原隔帧推进一段64步的速度一致
{
    static const uint8_t step_tbl[iSPEED_LEV_MAX+1] = {16, 8, 5, 4, 3};
    return step_tbl[speed_lev()];
}

static uint8_t mix8(bool a, bool b, uint16_t t) // 当前色通道a淡出、下一色通道b淡入, t=0~255
{
    return (uint8_t)((((a ? (256 - t) : 0) + (b ? t : 0)) * 255) >> 8);
}

static uint8_t fade8(uint8_t v, uint8_t k)      // 衰减 k/256, 至少减1, 保证能衰减到0
{
    uint8_t d = (uint8_t)((v * k) >> 8);
    return v - (d ? d : (v ? 1 : 0));
}

typedef struct {
    uint16_t frames;                            // 本次渐亮/渐暗已走帧数
    bool     rising;                            // true 渐亮, false 渐暗
} breath_t;

// 呼吸每帧走一步, 单程 len 帧(原64步×隔帧数); 返回亮度0~255; 由暗转亮的那一帧 *turned_up=true
static uint8_t breath_next(breath_t *b, uint16_t len, bool *turned_up)
{
    if(b->frames > len)
        b->frames = len;                        // 调速后单程变短
    uint8_t v = (uint8_t)(((uint32_t)b->frames * 255) / len);
    if(!b->rising)
        v = 255 - v;
    *turned_up = false;
    if(++b->frames > len)
    {
        b->frames = 0;
        b->rising = !b->rising;
        *turned_up = b->rising;
    }
    return v;
}

void bullet_light(void) {  //Bullet Light  子弹灯  彩虹小马跑  // 效果入口：单点跑马，按序切色
    static unsigned char inited;                                 // 是否已初始化（进入模式后首帧设置一次）
    static unsigned char stage;                                  // 当前颜色阶段索引（0..6：红绿蓝紫黄青白）
    static int16_t pos;                                          // 当前点亮的灯珠位置（0..iLED_MAX-1）
    static unsigned char manual_lock;                            // 颜色手动锁定标志（按下 Fn+Right 后为 1）
    static unsigned char prev_led_rgb_index;                     // 上一帧记录的颜色索引（用于检测手动切色）
    static bool prev_dir;                                        // 上一帧记录的方向（用于检测方向切换）
    bool first = (0xFF == delay_time_count);                     // 模式切换首帧, 须在清零前取出

    {
        unsigned char speed_lev = (unsigned char)led_speed_lev;
        if (speed_lev > iSPEED_LEV_MAX) {speed_lev = iSPEED_LEV_MAX;}
        unsigned short threshold = (unsigned short)((speed_lev + 2) << 4);  // 0档最快(32) ~ 4档最慢(96)
        if (threshold > delay_time_count) {
            delay_time_count++;
            return;
        }
    }
    delay_time_count = 0;                                        // 到推进时机：清零延时计数

    if (!inited || first) {                                      // 首次进入或模式切换首帧（入口复位）
        inited = 1;                                              // 标记已初始化
        stage = 0;                                               // 从序列第一个颜色开始（红）
        // pos = bled_run_dir ?  0  :   (iLED_MAX - 1) ;              // 根据方向设置起始灯位：右→左从最右开始；左→右从最左开始
            pos = bled_run_dir ? (iLED_MAX - 1) : 0;              // 根据方向设置起始灯位：右→左从最右开始；左→右从最左开始
        manual_lock = 0;                                         // 初始不锁定颜色（自动遍历颜色序列）
        prev_led_rgb_index = led_rgb_index;                      // 记录当前颜色索引（用于侦测 Fn+Right 手动切色）
        prev_dir = bled_run_dir;                                 // 记录当前方向（用于侦测 Fn+Left 切方向）
    }

    if (prev_led_rgb_index != led_rgb_index) {                   // 侦测到颜色索引变化（Fn+Right 手动切色）  led_rgb_index颜色全局变量
        manual_lock = (led_rgb_index < 7);                       // 7=七彩: 恢复自动遍历; 否则锁定颜色 (原 stage=7 越界读 seq[7])
        if (manual_lock) stage = led_rgb_index;                  // 切到手动选择的颜色阶段（0..6）
        prev_led_rgb_index = led_rgb_index;                      // 更新上一帧颜色索引
    }

    if (prev_dir != bled_run_dir) {                              // 侦测到方向变化（Fn+Left 切方向）  方向全局变量
        manual_lock = 0;                                         // 解除颜色锁定（恢复自动遍历颜色）
        prev_dir = bled_run_dir;                                 // 更新上一帧方向
    }

    static const unsigned char seq[7] = {                        // 颜色序列：红、绿、蓝、黄 紫、、青、白
        iPWM_R,
        iPWM_G,
        iPWM_B,
        (iPWM_R|iPWM_B),  //紫色
        (iPWM_R|iPWM_G),  //黄色
        (iPWM_G | iPWM_B),
        (iPWM_R | iPWM_G | iPWM_B)
    };

    rgb_enable = seq[stage];                                     // 选中当前阶段颜色作为“单点”显示的通道掩码

    for (gtemp_x = 0; gtemp_x < iLED_MAX; gtemp_x++) {           // 整排减半而不是清零: 走过的灯逐步变暗, 形成彗星拖尾
        pwm_rgb_tbl[gtemp_x][iR_INDEX] >>= 1;
        pwm_rgb_tbl[gtemp_x][iG_INDEX] >>= 1;
        pwm_rgb_tbl[gtemp_x][iB_INDEX] >>= 1;
    }

    if (pos >= 0 && pos < iLED_MAX) {
        unsigned char i0 = (unsigned char)pos;
        pwm_rgb_tbl[i0][iR_INDEX] = (rgb_enable & iPWM_R) ? iPWM_MAX : 0;
        pwm_rgb_tbl[i0][iG_INDEX] = (rgb_enable & iPWM_G) ? iPWM_MAX : 0;
        pwm_rgb_tbl[i0][iB_INDEX] = (rgb_enable & iPWM_B) ? iPWM_MAX : 0;
    }

    if (bled_run_dir) {
        pos += 1;
        if (pos >= iLED_MAX) {
            pos = 0;
            if (!manual_lock) {
                stage++;
                if (stage >= 7) stage = 0;
            }
        }
    } else {
        pos -= 1;
        if (pos < 0) {
            pos = (iLED_MAX - 1);
            if (!manual_lock) {
                stage++;
                if (stage >= 7) stage = 0;
            }
        }
    }
}


void qicai_sheng_shuo(void) {
    static unsigned char inited;
    static unsigned char stage;
    static unsigned char manual_lock;
    static unsigned char prev_led_rgb_index;
    static bool prev_dir;
    static unsigned char phase;
    static unsigned char count;
    bool first = (0xFF == delay_time_count);                     // 模式切换首帧, 须在清零前取出
    unsigned short threshold = (unsigned short)((led_speed_lev + 1) * 10);  // 0档最快(10) ~ 4档最慢(50)
    bool step = (delay_time_count >= threshold);                 // 到点才推进灯条长度, 其余帧只画边缘过渡
    if(step) delay_time_count = 0;
    else delay_time_count++;

    if(!inited || first){
        inited = 1;
        stage = 0;
        manual_lock = 0;
        prev_led_rgb_index = led_rgb_index;
        prev_dir = bled_run_dir;
        phase = 0;
        count = 0;
    }

    if (prev_led_rgb_index != led_rgb_index) {
        manual_lock = (led_rgb_index < 7);                       // 7=七彩: 恢复自动遍历, 不再锁死在青色
        if (manual_lock) stage = led_rgb_index;
        prev_led_rgb_index = led_rgb_index;
    }

    if (prev_dir != bled_run_dir) {
        manual_lock = 0;
        prev_dir = bled_run_dir;
        phase = 0;
        count = 0;
    }


    static const unsigned char seq[8] = {                        // 八色序列：蓝、青、绿、紫、红、紫、蓝、青
        iPWM_B,                    // 蓝
        (iPWM_G|iPWM_B),           // 青
        iPWM_G,                    // 绿
        (iPWM_R|iPWM_B),           // 紫
        iPWM_R,                    // 红
        (iPWM_R|iPWM_B),           // 紫（重复）
        iPWM_B,                    // 蓝（重复）
        (iPWM_G|iPWM_B)            // 青（重复）
    };

    


    if(step){
        if(phase == 0){
            if(count < iLED_MAX) count++;
            else phase = 1;
        } else {
            if(count > 0) count--;
            else {
                phase = 0;
                if (!manual_lock) { stage++; if (stage >= 8) stage = 0; }
            }
        }
    }

    rgb_enable = seq[stage];

    {
        unsigned char frac = (unsigned char)((delay_time_count * iPWM_MAX) / threshold);  // 本步进度 0~255
        for(unsigned char pos = 0; pos < iLED_MAX; pos++){           // pos: 从灯条起点数起的位置
            unsigned char v = (pos < count) ? iPWM_MAX : 0;
            if((phase == 0) && (pos == count)) v = frac;             // 伸长: 前沿下一颗灯淡入
            if((phase == 1) && (pos + 1 == count)) v = iPWM_MAX - frac;  // 收缩: 末端灯淡出
            gtemp_x = bled_run_dir ? pos : (iLED_MAX - 1 - pos);     // 方向：1→左→右；0→右→左
            pwm_rgb_tbl[gtemp_x][iR_INDEX] = (rgb_enable & iPWM_R) ? v : 0;
            pwm_rgb_tbl[gtemp_x][iG_INDEX] = (rgb_enable & iPWM_G) ? v : 0;
            pwm_rgb_tbl[gtemp_x][iB_INDEX] = (rgb_enable & iPWM_B) ? v : 0;
        }
    }
}

void led_mode_nhjb(void)
{
    static unsigned char offset;
    static uint16_t step_tick;
    static const unsigned char color_seq[6] = {  // 红黄绿青蓝品红: 与原每次只升/降一个通道的顺序相同
        iPWM_R, (iPWM_R|iPWM_G), iPWM_G, (iPWM_G|iPWM_B), iPWM_B, (iPWM_R|iPWM_B)
    };

    if(0xFF == delay_time_count)
    {                                           // 模式切换首帧: 从红开始
        offset = 0;
        step_tick = 0;
    }
    delay_time_count = 0;

    uint16_t t = step_tick >> 2;                // 段内进度 0~255
    unsigned char c = color_seq[offset];
    unsigned char n = color_seq[(offset + 1) % 6];
    for(gtemp_x = 0; gtemp_x < iLED_MAX; gtemp_x++)
    {
        pwm_rgb_tbl[gtemp_x][iR_INDEX] = mix8(c & iPWM_R, n & iPWM_R, t);
        pwm_rgb_tbl[gtemp_x][iG_INDEX] = mix8(c & iPWM_G, n & iPWM_G, t);
        pwm_rgb_tbl[gtemp_x][iB_INDEX] = mix8(c & iPWM_B, n & iPWM_B, t);
    }

    step_tick += fine_step();
    if(step_tick >= iFINE_SEG)
    {
        step_tick -= iFINE_SEG;
        if(++offset >= 6)
            offset = 0;
    }
}


void led_mode_qicai_breath(void)
{
    static unsigned char inited;
    static unsigned char manual_lock;
    static unsigned char stage;
    static unsigned char prev_led_rgb_index;
    static bool prev_dir;
    static breath_t br;
    unsigned char i;
    bool turned_up;

    if(!inited || (delay_time_count == 0xFF)){
        inited = 1;
        manual_lock = 0;
        stage = 0;
        prev_led_rgb_index = led_rgb_index;
        prev_dir = bled_run_dir;
        rgb_index = 0;
        br.frames = 0;
        br.rising = false;                      // 从最亮开始渐暗
    }
    delay_time_count = 0;

    // 每帧都走; 单程 64×(档+3) 帧, 与原"隔(档+3)帧走1步、64步"一致, 并保留最快档的下限
    unsigned char pwm_value = breath_next(&br, 64 * (speed_lev() + 3), &turned_up);
    if(turned_up && !manual_lock){
        rgb_index++;
        if(rgb_index >= 7)
            rgb_index = 0;
    }

    if(prev_led_rgb_index != led_rgb_index){
        manual_lock = (led_rgb_index < 7);      // 7=七彩: 恢复自动换色; 原 stage=7 取到 seq[7]=0 整排熄灭
        if(manual_lock) stage = led_rgb_index;
        prev_led_rgb_index = led_rgb_index;
    }

    if(manual_lock && (prev_dir != bled_run_dir)){
        manual_lock = 0;
        prev_dir = bled_run_dir;
    }

    {
        static const unsigned char seq[8] = {  
        iPWM_R,                // 红
        iPWM_G,                // 绿
        iPWM_B,                    // 蓝
        (iPWM_R|iPWM_B),           // 紫
        (iPWM_R|iPWM_G),           //黄色
        (iPWM_G|iPWM_B),           // 青
        (iPWM_G|iPWM_B|iPWM_R)      //白
    };

        uint8_t step = manual_lock ? stage : (rgb_index % 7);
        rgb_enable = seq[step];
    }

    for(i=0; i<iLED_MAX; i++)
    {
        pwm_rgb_tbl[i][iR_INDEX] = (rgb_enable&iPWM_R) ? pwm_value : 0;
        pwm_rgb_tbl[i][iG_INDEX] = (rgb_enable&iPWM_G) ? pwm_value : 0;
        pwm_rgb_tbl[i][iB_INDEX] = (rgb_enable&iPWM_B) ? pwm_value : 0;
    }
}

void led_mode_LYKS(unsigned char start){

    static unsigned char inited; // 初始化标志，首次或复位时置1
    static unsigned char stage; // 当前颜色阶段索引，0..6循环
    static int16_t rip_start[8]; // 波源起始位置数组
    static unsigned char rip_radius[8]; // 外圈半径，右侧扩散指针
    static unsigned char rip_active[8]; // 波源是否激活标志
    static uint16_t rip_tick[8]; // 每个波源的节拍累加计数
    static unsigned char rip_state[8]; // 每个波源的颜色阶段索引
    static int16_t rip_pos[8]; // 内圈位置，左侧扩散指针

    // 每帧都刷新(原无按键时隔(档+1)帧刷新一次, 尾迹衰减一顿一顿), 推进和衰减按档换算成每帧量, 速度与原来一致
    static const uint8_t move_tbl[iSPEED_LEV_MAX+1]  = {60, 30, 20, 15, 12};  // 每帧节拍, 满 iRIP_MOVE 推进一格(原每次+2满29)
    static const uint8_t decay_tbl[iSPEED_LEV_MAX+1] = {64, 34, 23, 18, 14};  // 每帧衰减 k/256, 累计(档+1)帧≈原每次衰减到3/4
    #define iRIP_MOVE   (29*30)

    if(!inited || (delay_time_count == 0xFF)){ // 首次进入或计数异常时，执行初始化
        inited = 1; // 标记已初始化
        stage = 0; // 颜色阶段复位到起始（红）
        for(unsigned char i=0; i<8; i++){ // 清空所有波源的状态
            rip_active[i] = 0; // 标记为空闲
            rip_radius[i] = 0; // 半径复位
            rip_tick[i] = 0; // 节拍复位
            rip_state[i] = 0; // 颜色阶段复位
            rip_pos[i] = 0; // 内圈位置复位
        }
        for(gtemp_x=0; gtemp_x<iLED_MAX; gtemp_x++){ // 清空显示缓冲，所有灯灭
            pwm_rgb_tbl[gtemp_x][iR_INDEX] = 0; // R通道关
            pwm_rgb_tbl[gtemp_x][iG_INDEX] = 0; // G通道关
            pwm_rgb_tbl[gtemp_x][iB_INDEX] = 0; // B通道关
        }
    }

    if(start < iLED_MAX){ // 有合法的触发位置（波源）
        unsigned char idx = 0xFF; // 预置无可用槽
        for(unsigned char i=0; i<8; i++){ // 从并行槽中寻找空闲波源
            if(!rip_active[i]){ idx = i; break; } // 找到空槽，记录并退出
        }
        if(idx != 0xFF){ // 找到可用槽，初始化该波源
            rip_start[idx] = (int16_t)start; // 记录波源起点
            rip_pos[idx] = (int16_t)start - 1; // 内圈从起点左侧一格开始
            rip_radius[idx] = (unsigned char)((unsigned char)start + 1); // 外圈从起点右侧一格开始
            rip_tick[idx] = 0; // 清零节拍
            rip_state[idx] = stage; // 使用当前颜色阶段
            rip_active[idx] = 1; // 激活该波源
            stage++; if(stage >= 7) stage = 0; // 下一次触发时切到下一颜色
        }
    }
    delay_time_count = 0;

    static const unsigned char color_seq[7] = { // 七色序列，依次红黄绿青蓝品红白    红蓝紫白绿黄青		
        iPWM_R, // 红
        iPWM_B, // 蓝
        (iPWM_R|iPWM_B), // 品红
        (iPWM_R|iPWM_G|iPWM_B), // 白（全开）
        iPWM_G, // 绿
        (iPWM_R|iPWM_G), // 黄
        (iPWM_G|iPWM_B) // 青
    };
    

    uint8_t k = decay_tbl[speed_lev()];
    for(gtemp_x=0; gtemp_x<iLED_MAX; gtemp_x++){ // 衰减而不是清屏: 涟漪走过后渐隐, 能衰减到0
        pwm_rgb_tbl[gtemp_x][iR_INDEX] = fade8(pwm_rgb_tbl[gtemp_x][iR_INDEX], k);
        pwm_rgb_tbl[gtemp_x][iG_INDEX] = fade8(pwm_rgb_tbl[gtemp_x][iG_INDEX], k);
        pwm_rgb_tbl[gtemp_x][iB_INDEX] = fade8(pwm_rgb_tbl[gtemp_x][iB_INDEX], k);
    }

    { // 绘制每个激活的涟漪波源
        for(unsigned char i=0; i<8; i++){ // 遍历所有并行波源
            if(!rip_active[i]) continue; // 跳过未激活的波源
            {
                int16_t lp = rip_pos[i]; // 当前内圈位置（向左扩散）
                int16_t rp = (int16_t)rip_radius[i]; // 当前外圈位置（向右扩散）
                unsigned char mask = color_seq[rip_state[i]]; // 波源使用的颜色掩码
                unsigned char val = (unsigned char)(((unsigned int)iPWM_MAX * (unsigned int)(5)) / (unsigned int)5); // 按亮度档映射到PWM值
                unsigned char val2 = val; // 备份一份亮度值
                if(lp >= 0 && lp < (int16_t)iLED_MAX){ // 内圈在有效范围
                    if(mask & iPWM_R) pwm_rgb_tbl[(unsigned char)lp][iR_INDEX] = val2; else pwm_rgb_tbl[(unsigned char)lp][iR_INDEX] = 0; // 内圈R通道赋值/清零
                    if(mask & iPWM_G) pwm_rgb_tbl[(unsigned char)lp][iG_INDEX] = val2; else pwm_rgb_tbl[(unsigned char)lp][iG_INDEX] = 0; // 内圈G通道赋值/清零
                    if(mask & iPWM_B) pwm_rgb_tbl[(unsigned char)lp][iB_INDEX] = val2; else pwm_rgb_tbl[(unsigned char)lp][iB_INDEX] = 0; // 内圈B通道赋值/清零
                }
                if(rp >= 0 && rp < (int16_t)iLED_MAX){ // 外圈在有效范围
                    if(mask & iPWM_R) pwm_rgb_tbl[(unsigned char)rp][iR_INDEX] = val2; else pwm_rgb_tbl[(unsigned char)rp][iR_INDEX] = 0; // 外圈R通道赋值/清零
                    if(mask & iPWM_G) pwm_rgb_tbl[(unsigned char)rp][iG_INDEX] = val2; else pwm_rgb_tbl[(unsigned char)rp][iG_INDEX] = 0; // 外圈G通道赋值/清零
                    if(mask & iPWM_B) pwm_rgb_tbl[(unsigned char)rp][iB_INDEX] = val2; else pwm_rgb_tbl[(unsigned char)rp][iB_INDEX] = 0; // 外圈B通道赋值/清零
                }
            }
            rip_tick[i] += move_tbl[speed_lev()]; // 节拍累加
            if(rip_tick[i] >= iRIP_MOVE){ // 达到推进门限时移动波源
                rip_tick[i] = 0; // 清零节拍
                rip_pos[i]--;  // 内圈向左移动一步
                rip_radius[i]++;  // 外圈向右移动一步
                if((rip_pos[i] < 0) && (rip_radius[i] >= iLED_MAX)){ // 两端都越界，认为该波源寿终正寝
                    rip_active[i] = 0; // 关闭该波源    
                }
            }
        }
    }
}

void led_mode_single(void)
{
    static const unsigned char seq[7] = {
        (iPWM_R|iPWM_G|iPWM_B),
        iPWM_R,
        (iPWM_R|iPWM_G), 
        iPWM_G,
        (iPWM_G|iPWM_B),
        iPWM_B,
        (iPWM_R|iPWM_B), 
    };
    
    rgb_enable = seq[led_rgb_index % 7 ];  

    for(gtemp_j=0; gtemp_j<iLED_MAX; gtemp_j++)
    {
        rgb_w_max();
    }
}

void led_mode_on(void)
{
    rgb_enable = rgb_en_tbl[led_rgb_index&0x07];
    for(gtemp_j=0; gtemp_j<iLED_MAX; gtemp_j++)
    {
        if(iRGB_QICAI == led_rgb_index)
            rgb_enable = rgb_en_tbl[gtemp_j&0x07];
        rgb_w_max();
    }
}

void led_mode_QCLG(void) {
    static unsigned char inited;
    static unsigned char offset;       // 时间偏移（始终递增）
    static uint16_t step_tick;         // 当前段内相位 0 ~ iFINE_SEG-1
    bool first = (0xFF == delay_time_count);           // 模式切换首帧, 须在清零前取出
    delay_time_count = 0;

    if (!inited || first) {
        inited = 1;
        offset = 0;
        step_tick = 0;
    }

    // 六色色环（RGB各通道只有开/关, 品红与紫无法区分, 去掉重复的紫）
    static const unsigned char color_seq[] = {
        iPWM_R | iPWM_G,   // 黄
        iPWM_R,            // 红
        iPWM_R | iPWM_B,   // 品红
        iPWM_B,            // 蓝
        iPWM_G | iPWM_B,   // 青
        iPWM_G             // 绿
    };
    const unsigned char color_num = sizeof(color_seq);

    const uint16_t phase_step = iFINE_SEG / iLED_MAX;  // 每个LED的相位跨度

    // 时间步进（offset 只增不减，保证切换方向时不跳变）
    step_tick += fine_step() << (iSPEED_LEV_MIN == led_speed_lev);   // 0档原来每次走2步
    if (step_tick >= iFINE_SEG) {
        step_tick -= iFINE_SEG;
        offset++;
        if (offset >= color_num) offset = 0;
    }

    // 每个LED独立计算流光颜色, 反向时镜像写入
    for (gtemp_x = 0; gtemp_x < iLED_MAX; gtemp_x++) {
        uint16_t phase_acc = step_tick + gtemp_x * phase_step;     // 相位累加：时间 + LED位置 × 相位步进
        uint16_t t = (phase_acc % iFINE_SEG) >> 2;                  // 段内进度 0~255
        unsigned char color_index = (unsigned char)((gtemp_x + offset + phase_acc / iFINE_SEG) % color_num);
        unsigned char c = color_seq[color_index];
        unsigned char n = color_seq[(color_index + 1) % color_num];
        unsigned char x = bled_run_dir ? (iLED_MAX - 1 - gtemp_x) : gtemp_x;
        pwm_rgb_tbl[x][iR_INDEX] = mix8(c & iPWM_R, n & iPWM_R, t);
        pwm_rgb_tbl[x][iG_INDEX] = mix8(c & iPWM_G, n & iPWM_G, t);
        pwm_rgb_tbl[x][iB_INDEX] = mix8(c & iPWM_B, n & iPWM_B, t);
    }
}

void led_mode_BBDY(void){
    static unsigned char inited; // 初始化标志
    static unsigned char offset; // 颜色序列偏移量
    static uint16_t bbdy_step_tick; // 换色进度 0 ~ iFINE_SEG-1
    bool first = (0xFF == delay_time_count); // 模式切换首帧, 须在清零前取出
    delay_time_count = 0;                    // 每帧都推进(fine_step), 不再隔帧节流

    if(!inited || first) // 首次或模式切换首帧重置
    {
        inited = 1; // 标记已初始化
        offset = 0; // 序列偏移复位
        bbdy_step_tick = 0; // 步进计数清零
    }

    {

        static const unsigned char color_seq[6] = { // 八色序列：蓝、青、绿、紫、红、紫、蓝、青
            iPWM_B,                    // 蓝
            (iPWM_G|iPWM_B),           // 青
            iPWM_G,                    // 绿
            (iPWM_R|iPWM_B),           // 紫
            iPWM_R,                    // 红
            (iPWM_R|iPWM_B),           // 紫（重复）

        };

        uint16_t t = bbdy_step_tick >> 2;    // 换色进度 0~255, 满一段时offset+1, 正好接上下一色
        for(gtemp_x = 0; gtemp_x < iLED_MAX; gtemp_x++) // 遍历所有灯位
        {
            unsigned char m = color_seq[(gtemp_x + offset) % 6];     // 当前色
            unsigned char n = color_seq[(gtemp_x + offset + 1) % 6]; // 下一色, 由当前色淡出、下一色淡入
            pwm_rgb_tbl[gtemp_x][iR_INDEX] = mix8(m & iPWM_R, n & iPWM_R, t);
            pwm_rgb_tbl[gtemp_x][iG_INDEX] = mix8(m & iPWM_G, n & iPWM_G, t);
            pwm_rgb_tbl[gtemp_x][iB_INDEX] = mix8(m & iPWM_B, n & iPWM_B, t);
        }
    }

    bbdy_step_tick += fine_step() << (iSPEED_LEV_MIN == led_speed_lev);   // 0档原来每次走2步
    if(bbdy_step_tick < iFINE_SEG) // 未满一段则返回
        return; // 下一帧继续
    bbdy_step_tick -= iFINE_SEG; // 满一段

    offset++; // 偏移+1，实现滚动
    if(offset >=6 ) offset = 0; // 环回到起点
}

void led_mode_breath(void)
{
    static breath_t br;
    unsigned char i;
    bool turned_up;

    if(0xFF == delay_time_count)
    {                                           // 模式切换首帧: 从最亮开始渐暗
        br.frames = 0;
        br.rising = false;
    }
    delay_time_count = 0;

    // 每帧都走; 单程 64×(档+1) 帧, 与原"隔(档+1)帧走1步、64步"一致
    unsigned char pwm_value = breath_next(&br, 64 * (speed_lev() + 1), &turned_up);
    if(turned_up)
        rgb_index++;

    rgb_enable = rgb_en_tbl[led_rgb_index&0x07];

    for(i=0; i<iLED_MAX; i++)
    {
        if(iRGB_QICAI == led_rgb_index)
        {
            rgb_enable = ((rgb_index+i)&0x07);
            if(0==rgb_enable)
                rgb_enable = 4;
        }

        if(rgb_enable&iPWM_R)
            pwm_rgb_tbl[i][iR_INDEX] = pwm_value;
        else
            pwm_rgb_tbl[i][iR_INDEX] = 0;
        if(rgb_enable&iPWM_G)
            pwm_rgb_tbl[i][iG_INDEX] = pwm_value;
        else
            pwm_rgb_tbl[i][iG_INDEX] = 0;
        if(rgb_enable&iPWM_B)
            pwm_rgb_tbl[i][iB_INDEX] = pwm_value;
        else
            pwm_rgb_tbl[i][iB_INDEX] = 0;
    }
}

