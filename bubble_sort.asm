.data
array: .word 7, 3, 5, 1, 6, 2, 4, 0  # 初始数组

.text
main:

    # 初始化寄存器
    la $t0,array  # $t0 = array的地址
    addi $t1, $zero, 8      # $t1 = 数组大小
    addi $t2, $zero, 0      # $t2 = 外层循环计数器 i
outer_loop:
    beq $t2, $t1, done      # 如果 i == 数组大小，跳转到 done
    addi $t3, $zero, 0      # $t3 = 内层循环计数器 j
inner_loop:
    beq $t3, $t1, outer_done # 如果 j == 数组大小，跳转到 outer_done
    # 加载 array[j] 和 array[j+1]
    lw $t4, 0($t0)          # $t4 = array[j]
    lw $t5, 4($t0)          # $t5 = array[j+1]
    # 比较 array[j] 和 array[j+1]
    slt $t6, $t4, $t5       # $t6 = (array[j] < array[j+1]) ? 1 : 0
    beq $t6, $zero, skip_swap # 如果 array[j] >= array[j+1]，跳过交换
    # 交换 array[j] 和 array[j+1]
    sw $t5, 0($t0)          # array[j] = array[j+1]
    sw $t4, 4($t0)          # array[j+1] = array[j]
skip_swap:
    addi $t0, $t0, 4        # $t0 += 4 (指向下一个元素)
    addi $t3, $t3, 1        # j++
    j inner_loop            # 跳转到 inner_loop
outer_done:
    addi $t2, $t2, 1        # i++
    la   $t0,array  # 重置 $t0 到数组起始地址
    j outer_loop            # 跳转到 outer_loop
done:
    # 排序完成，数组现在存储在 array 中
    # 可以在这里添加代码来打印数组或执行其他操作
