(define (vector_add v1 v2)
  (if (equal_vec_size? v1 v2)
    (_vector_add v1 v2)
    "incompatible"))

(define (equal_vec_size? v1 v2)
  (if (= (length v1) (length v2))
    #t
    #f))

(define (_vector_add v1 v2)
  (if (not (null? v1))
    (cons
        (+ (car v1) (car v2))
        (_vector_add
          (cdr v1)
          (cdr v2)))
    ()))

(define (gen_seq start step len)
  (if (not (= len 0))
    (cons
      start
      (gen_seq 
        (+ start step) step (- len 1)))
    ()))

(vector_add (gen_seq 0 1 10) (gen_seq 1 1 10))