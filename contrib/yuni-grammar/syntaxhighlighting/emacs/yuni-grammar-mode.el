;; Emacs major mode for Yuni Grammar. Written by the Yuni project team.
;; Free to use, redistribute, and modify freely.
;; No implied warranty of any kind.
;;
;; You may add the following to your init.el :
;; (autoload 'yuni-grammar-mode "/path/to/yuni-grammar-mode.el" "Major mode for editing Yuni grammars." t)


(defgroup yuni-grammar nil
	"Major mode for editing Yuni grammars."
																				;	 :link '(custom-group-link :tag "Font Lock Faces group" font-lock-faces)
	:prefix "yuni-grammar-"
	:group 'languages
	)

(defvar yuni-grammar-mode-hook nil "List of functions to call when entering Yuni-Grammar mode")

;;;###autoload
(add-to-list 'auto-mode-alist '("\\.ygr\\'" . yuni-grammar-mode))


;; Syntax table
(defvar yuni-grammar-mode-syntax-table
	(let ((table (make-syntax-table c-mode-syntax-table)))
		(modify-syntax-entry ?\( "()" table)
		(modify-syntax-entry ?\) ")(" table)
		(modify-syntax-entry ?\[ "(]" table)
		(modify-syntax-entry ?\] ")[" table)
		(modify-syntax-entry ?\{ "(}" table)
		(modify-syntax-entry ?\} "){" table)
		table
		)
	)


;; Keywords
(defconst yuni-grammar-keywords
	'("important" "notext" "inline" "hidden")
	"Yuni grammar keywords"
	)
;; Regexped version
(defvar yuni-grammar-keywords-regexp (regexp-opt yuni-grammar-keywords 'words))


;; Operators
(defconst yuni-grammar-operators
	'("+" "?" "*" "|")
	"Yuni grammar operators"
	)
;; Regexped version
(defvar yuni-grammar-operators-regexp (regexp-opt yuni-grammar-operators))

;; Special Operators
(defconst yuni-grammar-special-operators
	'(":")
	"Yuni grammar special operators"
	)
;; Regexped version
(defvar yuni-grammar-special-operators-regexp (regexp-opt yuni-grammar-special-operators))


;; Syntax highlighting
(defvar yuni-grammar-font-lock-keywords
	`(
		;; Strings
		("\".*\"" 0 font-lock-string-face)
		("\'.*\'" 0 font-lock-string-face)
		;; Comments
		("//.*$" . font-lock-comment-face)
		;; Operators
		(,yuni-grammar-operators-regexp . font-lock-keyword-face)
		(,yuni-grammar-special-operators-regexp . font-lock-warning-face)
		;; Rule
		("^\\([A-Za-z][A-Za-z0-9_-]*\\):" (1 font-lock-function-name-face))
		;; Tokens
		("[A-Za-z][A-Za-z0-9_-]*" . font-lock-variable-name-face)
		;; Keywords
		(,yuni-grammar-keywords-regexp . font-lock-keyword-face)
		;; Numbers
		("[0-9]+" . font-lock-constant-face)
		)
	"Syntax highlighting in Yuni Grammar mode"
	)



;; Command to comment/uncomment text
(defun yuni-grammar-comment-dwim (arg)
	"Comment or uncomment current line or region in a smart way.
	For detail, see `comment-dwim'."
	(interactive "*P")
	(require 'newcomment)
	(let ((deactivate-mark nil) (comment-start "//") (comment-end ""))
		(comment-dwim arg)
		)
	)


;; Define the major mode
(defun yuni-grammar-mode ()
	"Major mode for editing Yuni grammars."
	(interactive)
	(kill-all-local-variables)
	;; Major mode definition
	(setq major-mode 'yuni-grammar-mode)
	;; Mode name
	(setq mode-name "Yuni Grammar")
	;; Syntax table
	(set-syntax-table yuni-grammar-mode-syntax-table)
	;; Comments
	(set (make-local-variable 'comment-start) "//")
	(set (make-local-variable 'comment-end) "")
	;; Use tab indents
	(set (make-local-variable 'indent-tabs-mode) t)
	;; Font lock
	(make-local-variable 'font-lock-defaults)
	(setq font-lock-defaults '(yuni-grammar-font-lock-keywords t))
	;; Hooks
	(run-mode-hooks 'yuni-grammar-mode-hook)
	)

(provide 'yuni-grammar-mode)
