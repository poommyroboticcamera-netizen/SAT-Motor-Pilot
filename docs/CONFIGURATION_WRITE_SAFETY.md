# Toolbar configuration write transaction

The desktop toolbar motor Write and application Write actions now require a connected controller, completed firmware handshake, non-limited commands, no deserialization failure since connection, and an available UUID. They retain a copy of the edited values, request the current device configuration, and save that fresh configuration as an XML backup under the separate MotorLab application-data directory. A failed read or backup blocks sending.

Confirmation identifies port, UUID and CAN forwarding target, lists the number of changed parameters, and exposes each old/new value in its details. Motor current-margin, slow-ABS-current and overmodulation cautions preserve the conditions of upstream motor sanity checks. Cancellation sends no write. Already-matching values cause no write.

After confirmation, the transaction sends once, waits up to five seconds for the matching motor/application acknowledgement, then requests fresh readback and compares the exact upstream serialized configuration against the reviewed values. Success is reported only after matching readback. Missing ACK, target changes or failed readback are reported as uncertain; the code never automatically repeats a write. Mismatches show actual versus expected parameter values and retain actual device values in the interface. Outcomes are appended to `config-write-audit.jsonl` under MotorLab application data, and inability to open/flush the pre-send audit record blocks writing.

The upstream motor transport's immediate check/read is disabled for these two actions because this transaction owns the ACK-then-readback sequence. Its three sanity conditions are checked before confirmation instead. Event-loop waits exclude user-input events and a shared guard rejects reentry into either toolbar transaction. Modal confirmation blocks interaction with its parent window.

## Scope and verification limits

This change covers only the two desktop toolbar/menu actions. Wizards, CLI, scripting, detection, custom configuration and other upstream write entry points retain their upstream behavior and are outside this guard. This is a documented coverage gap, not a claim of complete write safety.

The protocol provides no per-request transaction identifier for these configuration responses. Matching target identity and ACK type cannot prove that a late reply belongs to a particular request. The current code also does not establish electrical safety, validate hardware limits against an identified board, or certify motor stopping. No hardware tests were performed for this change. Compile/offline test results must be recorded by the build validation task; those results cannot substitute for controller-side ACK/readback testing, transport-disconnect tests, or backup restoration tests.
