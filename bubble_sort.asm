
.text
main:
    # 加载数组地址和长度
    lui      $a0, 0x00        # $a0 = 数组地址
    lui      $a1, 9 # $a1 = 数组长度

    # 调用冒泡排序
    jal     bubble_sort       # 调用冒泡排序


# 冒泡排序程序
# 输入：数组存储在内存地址 $a0 开始的位置
#       数组长度存储在 $a1
# 输出：排序后的数组存储在原位置

# 冒泡排序主程序
bubble_sort:
    # 初始化外层循环计数器 i = 0
    addi    $t0, $zero, 0       # $t0 = 0 (外层循环计数器 i)

outer_loop:
    # 比较 i 和 数组长度，判断是否结束外层循环
    sub     $t6, $a1, $t0       # $t6 = 数组长度 - i
    slti    $t6, $t6, 1         # 如果 $t6 < 1，则 $t6 = 1，否则 $t6 = 0
    bne     $t6, $zero, outer_next  # 如果 $t6 != 0，即 i < 数组长度，继续外层循环

    # 初始化内层循环计数器 j = 0
    addi    $t1, $zero, 0       # $t1 = 0 (内层循环计数器 j)

inner_loop:
    # 比较 j 和 数组长度，判断是否结束内层循环
    sub     $t6, $a1, $t1       # $t6 = 数组长度 - j
    slti    $t6, $t6, 1         # 如果 $t6 < 1，则 $t6 = 1，否则 $t6 = 0
    bne     $t6, $zero, inner_next  # 如果 $t6 != 0，即 j < 数组长度，继续内层循环

    # 计算数组[j] 和数组[j+1] 的地址
    add     $t2, $a0, $t1       # $t2 = 数组[j] 的地址
    addi    $t2, $t2, 4         # $t2 = 数组[j+1] 的地址

    # 加载数组[j] 和数组[j+1] 的值
    lw      $t3, 0($t2)         # $t3 = 数组[j+1]
    lw      $t4, -4($t2)        # $t4 = 数组[j]

    # 比较数组[j] 和数组[j+1]，判断是否需要交换
    sub     $t6, $t4, $t3       # $t6 = 数组[j] - 数组[j+1]
    slti    $t6, $t6, 0         # 如果 $t6 < 0，则 $t6 = 1，否则 $t6 = 0
    bne     $t6, $zero, inner_next  # 如果 $t6 != 0，即 数组[j] < 数组[j+1]，跳过交换

    # 交换数组[j] 和数组[j+1]
    sw      $t3, -4($t2)        # 数组[j] = 数组[j+1]
    sw      $t4, 0($t2)         # 数组[j+1] = 数组[j]

inner_next:
    # j++
    addi    $t1, $t1, 4         # $t1 += 4
    j       inner_loop          # 继续内层循环

outer_next:
    # i++
    addi    $t0, $t0, 4         # $t0 += 4
    j       outer_loop          # 继续外层循环

finish:
    jr      $ra                 # 返回调用者

