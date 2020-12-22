import main

if __name__ == '__main__':
    cnt = 10000

    s = main.Searcher(cnt)
    print('Searcher init.')
    err = 0
    print('Brute search test:', end='')
    for i in range(cnt):
        if s.brute(100000 + i) != s.array[i]:
            err += 1
    if err:
        print(err)
    else:
        print('OK')
    err = 0
    print('Binary search test:', end='')
    for i in range(cnt):
        if s.binary(100000 + i) != s.array[i]:
            err += 1
    if err:
        print(err)
    else:
        print('OK')

    err = 0
    s.prepare_seg()
    print('Segment search test:', end='')
    for i in range(cnt):
        if s.segment(100000 + i) != s.array[i]:
            err += 1
    if err:
        print(err)
    else:
        print('OK')