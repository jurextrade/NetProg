(defun runsc ()

(remote pcom (ps))
(get pcom "ps.file")
(system "vi ps.file")
)

