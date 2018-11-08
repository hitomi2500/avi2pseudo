// �ਡ����� � 32-� ��⭮�� ��� 16-��⭮�

void add32_16(ulong* a, ushort b) {
  asm {
    ; de = *to
    lhld add32_16_1
    mov e, m
    inx h
    mov d, m

    ; hl = de + from
    lhld add32_16_2
    dad d

    ; *hl = de
    xchg
    lhld add32_16_1
    mov m, e
    inx h
    mov m, d     

    ; �᫨ ��९�������, � 㢥��稢��� ���孨� ࠧ��
    rnc
    inx h
    inr m    

    ; �᫨ ��९�������, � 㢥��稢��� ���孨� ࠧ��
    rnz
    inx h
    inr m
  }
}  