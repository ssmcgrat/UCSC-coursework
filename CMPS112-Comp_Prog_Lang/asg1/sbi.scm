#!/afs/cats.ucsc.edu/courses/cmps112-wm/usr/racket/bin/mzscheme -qr
;; $Id: sbi.scm,v 1.2 2015-09-23 17:11:09-07 - - $
;;
;; Authors:
;;    Sean McGrath (ssmcgrat@ucsc.edu)
;;    Tim Mertogul (tmertogu@ucsc.edu)	
;;
;; NAME
;;    sbi.scm - silly basic interpreter
;;
;; SYNOPSIS
;;    sbi.scm filename.sbir
;;
;; DESCRIPTION
;;    The file mentioned in argv[1] is read and assumed to be an
;;    SBIR program, which is the executed.  Currently it is only
;;    printed.
;;

(define *stderr* (current-error-port))

;; We define an atom as anything that is not null and is not a pair
;;
(define (atom? x)
    (and (not (pair? x))
        (not (null? x))))

;; Make printing a little easier
;;
(define (show label it)
    (display label)
    (display " = ")
    (display it)
    (newline))

;; Evaluate expressions
(define (expr_eval expr)
    (cond
       ((string? expr) expr)
       ((number? expr) expr)

     )
)

;; Our print function for dealing with the 'print' statement
;;
(define (sb_print printable)
    (display printable)) 


;; This is the hash table for storing each label and their 
;; respective keys
;;
(define *label-table* (make-hash))

;; This is the table to hold all of the functions, which include the
;; operators as well. Gets initialized when the program begins. 
(define *function-table* (make-hash))

;; Put pairs into *function-table*
(define (function-put! key value)
        (hash-set! *function-table* key value))

;; Procedure called upon to insert all function into *function-table*
;;
(for-each
    (lambda (pair)
            (function-put! (car pair) (cadr pair)))
    `(

        (log10_2 0.301029995663981195213738894724493026768189881)
        (sqrt_2  1.414213562373095048801688724209698078569671875)
        (e       2.718281828459045235360287471352662497757247093)
        (pi      3.141592653589793238462643383279502884197169399)
        (div     ,(lambda (x y) (floor (/ x y))))
        (log10   ,(lambda (x) (/ (log x) (log 10.0))))
        (mod     ,(lambda (x y) (- x (* (div x y) y))))
        (quot    ,(lambda (x y) (truncate (/ x y))))
        (rem     ,(lambda (x y) (- x (* (quot x y) y))))
        (+       ,+)
        (^       ,expt)
        (ceil    ,ceiling)
        (exp     ,exp)
        (floor   ,floor)
        (log     ,log)
        (sqrt    ,sqrt)
        (*       ,*)
        (/       ,/)
        (-       ,-)
        (atan    ,atan)
        (print   ,print) 
        (=       ,=)
        (<       ,<)
        (>       ,>)
        (>=      ,>=)
        (<=      ,<=) 
    ))

;; Define our *variable-table*
(define *variable-table* (make-hash))

;; Put pairs into *variable-table*
(define (variable-put! key value)
        (hash-set! *variable-table* key value))

;; Insert all variables into *variable-table*
;;
(for-each
    (lambda (pair)
            (variable-put! (car pair) (cadr pair)))
    `(
         (e       2.718281828459045235360287471352662497757247093)
         (pi      3.141592653589793238462643383279502884197169399)       
         (my_new_first 0)
         (my_new_second 0)
    ))
(define *run-file*
    (let-values
        (((dirpath basepath root?)
            (split-path (find-system-path 'run-file))))
        (path->string basepath))
)

(define (die list)
    (for-each (lambda (item) (display item *stderr*)) list)
    (newline *stderr*)
    (exit 1)
)

(define (usage-exit)
    (die `("Usage: " ,*run-file* " filename"))
)

;; Reads the input file line-by-line into a list called 'program'
;; Each element of the list is a statement
;;
(define (readlist-from-inputfile filename)
    (let ((inputfile (open-input-file filename)))
         (if (not (input-port? inputfile))
             (die `(,*run-file* ": " ,filename ": open failed"))
             (let ((program (read inputfile)))
                  (close-input-port inputfile)
                         program))))


;; Writes the entire program to stdout line-by-line
;;
(define (write-program-by-line filename program)
    (printf "==================================================~n")
    (printf "~a: ~s~n" *run-file* filename)
    (printf "==================================================~n")
    (printf "~n")
    (map (lambda (line) (printf "~s~n" line)) program)
    (printf "~n"))

;; Finds all labels in program, if any, and inserts them into 
;; *label-table*. Note that the label is the cadr of each line in 
;; program, that is, iff the cdr is not null. 
;;
(define (find-labels-and-hash filename program)
    (map (lambda (line)
        (when (not (null? (cdr line)))
            (when (atom? (cadr line))
                (hash-set! *label-table* (cadr line)  line)))) program)  

    ;; print out the hash table
    ;;(hash-for-each *label-table* (lambda (key value) (show key value)))
)

;; hash table for testing John's recursion code
;;
(define hash (make-hash))
(hash-set! hash + (lambda (x) (+ (car x) (cadr x))))
(hash-set! hash - (lambda (x) (- (car x) (cadr x))))
(hash-set! hash * (lambda (x) (* (car x) (cadr x))))
(hash-set! hash / (lambda (x) (/ (car x) (cadr x))))
(hash-set! hash expt (lambda (x) (expt (car x) (cadr x))))

;; John's recurssion function
;;
(define (evall list)
   
   ;; playing with pattern matching

 
   ;; get the values 
   (let ([func (hash-ref *function-table* (car list))] [first (cadr list)] [second (caddr list)])
      
       (cond 
           ((and (not (list? first)) (not (list? second)))
            (when (not (number? first))
                (set! first (hash-ref *variable-table* first))) 
            
            (when (not (number? second))
                (set! second (hash-ref *variable-table* second)))
             ((hash-ref hash func) `(,first, second)))

           ((and (not (list? first)) (list? second))
             (when (not (number? first))
                (set! first (hash-ref *variable-table* first)))           
 
              ((hash-ref hash func) `(,first, (evall second))))

           ((and (not (list? second)) (list? first)) 
              (when (not (number? second))
                (set! second (hash-ref *variable-table* second))) 

              ((hash-ref hash func) `(,(evall first), second)))       

           (else (let ([evalFirst (evall first)] [evalSecond (evall second)])
               ((hash-ref hash func) `(,evalFirst, evalSecond))))
           )
        )                   
)

       

;; recursive function to evaluate nested expressions such as
;; (+ (* 3 4) (*5 6))
;;
(define (execute-line l program line_num)
    ;(printf "~s~n" l) 
    (cond
         
        ;; if (car l) is 'print, and if (cdr l) is not null
        ;; we print the cadr followed by a newline 
        ((eq? (car l) 'print)
             (when (not (null? (cdr l)))
                  (sb_print (cadr l))
                  ;; check if there are functions to evaluate at 
                  ;; end of line 
                  (when (> (length (cdr l)) 1)
                      
                      (cond
                          ;; if caddr l is an atom, return its value
                          ((atom? (caddr l))
                             (printf "~s" (hash-ref *variable-table*
                                         (caddr l))))
 
                          ;; if not an atom, evaluate expression
                          (else
                              
                             (printf "~s" (evall (caddr l))))))
              
                  ;; always print a newline 
                  (newline))
             (eval-line program (+ line_num 1)))
        
        ;; if (car l) is 'let
        ((eq? (car l) 'let)
           ;; check if caddr l is an atom
           (cond
              ((atom? (caddr l))
                  (variable-put! (cadr l) (caddr l)))
              (else
                  (variable-put! (cadr l) (evall (caddr l))))) 
           
         (eval-line program (+ line_num 1)))
  
        ;; if (car l) is 'if
        ((eq? (car l) 'if)
           (printf "its if~n")

           (eval-line program (+ line_num 1)))
 
        ((eq? (car l) 'goto)
            (define found_line (car (hash-ref *label-table* (cadr l)))) 
            (eval-line program (- found_line 1))) 
        (else 
            (display (evall l))
            (newline)) 
     ) 

)
 
;; Execute program line-by-line. 
;; Takes a line number to execute. 
;;
(define (eval-line program line_num)
    ;; make sure the given line_num is not > than 
    ;; the number of lines in program  
    (when (> (length program) line_num)
  
        (let ((line (list-ref program line_num)))
            
            (cond
                ;; when the line length is 3
                ;; 
                ((= (length line) 3)
                    ;(printf "line length is 3: ~s~n" line) 
                    (set! line (cddr line))
                    (execute-line (car line) program line_num))

                ;; when length of line is 2 and cadr is a list
                ;;
                ((and (= (length line) 2) (list? (cadr line)))
                  
                 (set! line (cdr line))
                 ;(printf "line is: ~s~n" line)
                 (execute-line (car line) program line_num))
                
                ;; otherwise move onto next line
                ;;
                (else
                   (eval-line program (+ line_num 1)))                
             )
         )
     )
)

(define (main arglist)
    (if (or (null? arglist) (not (null? (cdr arglist))))
        (usage-exit)
        (let* ((sbprogfile (car arglist))
               (program (readlist-from-inputfile sbprogfile)))
             
              ;; here we call a function to find all labels, 
              ;; if any exist.
              (find-labels-and-hash sbprogfile program)
              
              ;;evaluate each line, beggining at line 1 
              (eval-line program 0)
)))

(main (vector->list (current-command-line-arguments)))

