// �ਡ����� � 32-� ��⭮�� ��� 16-��⭮�

void set32(ulong* a, ulong* b) {  
  asm {
    xchg ; de = set32_2
    lhld set32_1
    ldax d
    mov m, a
    inx h
    inx d
    ldax d
    mov m, a
    inx h
    inx d
    ldax d
    mov m, a
    inx h
    inx d
    ldax d
    mov m, a
  }
}  