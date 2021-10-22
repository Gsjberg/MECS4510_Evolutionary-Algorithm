function sim = similarity(heap1, heap2, data)
    sim = abs(eror_cal(heap1, data) - eror_cal(heap2, data));
end